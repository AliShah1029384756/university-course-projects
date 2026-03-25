import { FILE_TYPES, CHUNK_SIZE, FILE_TRANSFER_TIMEOUT, MAX_RETRIES, CHUNK_DELAY } from '../constants/fileTypes';

export class FileTransferManager {
    constructor() {
        this.activeTransfers = new Map(); // transferId -> transfer state
        this.fileReceivers = new Map(); // fileId -> receiver state
        this.onProgressCallback = null;
        this.onCompleteCallback = null;
        this.onErrorCallback = null;
        this.onFileReadyCallback = null;
    }

    setCallbacks(callbacks) {
        const { onProgress, onComplete, onError, onFileReady } = callbacks;
        this.onProgressCallback = onProgress;
        this.onCompleteCallback = onComplete;
        this.onErrorCallback = onError;
        this.onFileReadyCallback = onFileReady;
    }

    async sendFile(file, fileId, connections, peerId) {
        try {
            const buffer = await file.arrayBuffer();
            const totalChunks = Math.ceil(buffer.byteLength / CHUNK_SIZE);
            const uint8Array = new Uint8Array(buffer);
            
            const transfer = {
                fileId,
                fileName: file.name,
                fileSize: file.size,
                totalChunks,
                sentChunks: 0,
                status: 'sending'
            };
            
            this.activeTransfers.set(fileId, transfer);

            // If peerId is provided, send to specific peer, otherwise broadcast
            const targetConnections = peerId ? 
                new Map([[peerId, connections.get(peerId)]]) : 
                connections;

            const results = new Map();

            for (const [currentPeerId, conn] of targetConnections.entries()) {
                if (!conn || !conn.open) {
                    results.set(currentPeerId, false);
                    continue;
                }

                let retryCount = 0;
                let transferSuccess = false;

                while (retryCount < MAX_RETRIES && !transferSuccess) {
                    try {
                        // Send start message
                        conn.send({
                            type: FILE_TYPES.START,
                            fileId,
                            fileName: file.name,
                            fileSize: file.size,
                            totalChunks,
                            retryAttempt: retryCount + 1
                        });

                        // Send chunks with acknowledgment
                        for (let i = 0; i < totalChunks; i++) {
                            const chunk = uint8Array.slice(i * CHUNK_SIZE, (i + 1) * CHUNK_SIZE);
                            const chunkId = `${fileId}-chunk-${i}`;
                            
                            conn.send({
                                type: FILE_TYPES.CHUNK,
                                fileId,
                                chunkId,
                                chunkIndex: i,
                                data: chunk,
                                totalChunks
                            });

                            // Wait for chunk acknowledgment
                            const ackReceived = await this.waitForAcknowledgment(conn, chunkId);
                            if (!ackReceived) {
                                throw new Error(`Chunk ${i} acknowledgment timeout`);
                            }

                            transfer.sentChunks++;
                            const progress = Math.round((transfer.sentChunks / totalChunks) * 100);
                            
                            if (this.onProgressCallback) {
                                this.onProgressCallback(fileId, currentPeerId, progress);
                            }

                            await new Promise(resolve => setTimeout(resolve, CHUNK_DELAY));
                        }

                        // Send end message and wait for final acknowledgment
                        conn.send({
                            type: FILE_TYPES.END,
                            fileId
                        });

                        const finalAck = await this.waitForAcknowledgment(conn, `${fileId}-complete`);
                        if (finalAck) {
                            transferSuccess = true;
                            results.set(currentPeerId, true);
                        }
                    } catch (error) {
                        console.error(`Transfer attempt ${retryCount + 1} failed:`, error);
                        retryCount++;
                        await new Promise(resolve => setTimeout(resolve, 1000 * retryCount));
                    }
                }

                if (!transferSuccess) {
                    results.set(currentPeerId, false);
                }
            }

            const successCount = Array.from(results.values()).filter(Boolean).length;
            if (this.onCompleteCallback) {
                this.onCompleteCallback(fileId, successCount, results.size);
            }

            return results;
        } catch (error) {
            if (this.onErrorCallback) {
                this.onErrorCallback(fileId, error);
            }
            throw error;
        } finally {
            this.activeTransfers.delete(fileId);
        }
    }

    handleIncomingData(data, conn) {
        try {
            switch (data.type) {
                case FILE_TYPES.START:
                    this.initializeReceiver(data, conn);
                    break;
                case FILE_TYPES.CHUNK:
                    this.handleChunk(data, conn);
                    break;
                case FILE_TYPES.END:
                    this.finalizeTransfer(data, conn);
                    break;
                case FILE_TYPES.CHUNK_ACK:
                case FILE_TYPES.TRANSFER_COMPLETE:
                    // Handle in waitForAcknowledgment
                    break;
            }
        } catch (error) {
            console.error('Error handling incoming data:', error);
            if (this.onErrorCallback) {
                this.onErrorCallback(data.fileId, error);
            }
        }
    }

    initializeReceiver(data, conn) {
        const receiver = {
            fileName: data.fileName,
            fileSize: data.fileSize,
            totalChunks: data.totalChunks,
            receivedChunks: new Array(data.totalChunks),
            chunksReceived: 0,
            status: 'receiving'
        };
        
        this.fileReceivers.set(data.fileId, receiver);
        
        if (this.onProgressCallback) {
            this.onProgressCallback(data.fileId, conn.peer, 0);
        }
    }

    handleChunk(data, conn) {
        const receiver = this.fileReceivers.get(data.fileId);
        if (!receiver) return;

        receiver.receivedChunks[data.chunkIndex] = data.data;
        receiver.chunksReceived++;

        // Send acknowledgment
        conn.send({
            type: FILE_TYPES.CHUNK_ACK,
            fileId: data.fileId,
            chunkId: data.chunkId
        });

        const progress = Math.round((receiver.chunksReceived / receiver.totalChunks) * 100);
        if (this.onProgressCallback) {
            this.onProgressCallback(data.fileId, conn.peer, progress);
        }

        // If all chunks received, notify that file is ready for download
        if (receiver.chunksReceived === receiver.totalChunks) {
            if (this.onFileReadyCallback) {
                this.onFileReadyCallback(data.fileId, receiver.fileName, receiver.fileSize);
            }
        }
    }

    finalizeTransfer(data, conn) {
        const receiver = this.fileReceivers.get(data.fileId);
        if (!receiver || receiver.chunksReceived !== receiver.totalChunks) return;

        // Send final acknowledgment
        conn.send({
            type: FILE_TYPES.TRANSFER_COMPLETE,
            fileId: data.fileId
        });

        if (this.onCompleteCallback) {
            this.onCompleteCallback(data.fileId, 1, 1);
        }
    }

    downloadFile(fileId) {
        const receiver = this.fileReceivers.get(fileId);
        if (!receiver || receiver.chunksReceived !== receiver.totalChunks) {
            throw new Error('File not ready for download');
        }

        try {
            const combinedArray = new Uint8Array(receiver.fileSize);
            let offset = 0;
            receiver.receivedChunks.forEach(chunk => {
                combinedArray.set(chunk, offset);
                offset += chunk.length;
            });

            const blob = new Blob([combinedArray]);
            const url = URL.createObjectURL(blob);
            
            // Trigger download
            const a = document.createElement('a');
            a.href = url;
            a.download = receiver.fileName;
            document.body.appendChild(a);
            a.click();
            document.body.removeChild(a);
            URL.revokeObjectURL(url);

            // Clean up receiver state
            this.fileReceivers.delete(fileId);
        } catch (error) {
            console.error('Error downloading file:', error);
            if (this.onErrorCallback) {
                this.onErrorCallback(fileId, error);
            }
            throw error;
        }
    }

    waitForAcknowledgment(conn, id) {
        return new Promise((resolve) => {
            const timeout = setTimeout(() => {
                conn.off('data', handleAck);
                resolve(false);
            }, FILE_TRANSFER_TIMEOUT);

            const handleAck = (data) => {
                if ((data.type === FILE_TYPES.CHUNK_ACK && data.chunkId === id) ||
                    (data.type === FILE_TYPES.TRANSFER_COMPLETE && `${data.fileId}-complete` === id)) {
                    clearTimeout(timeout);
                    conn.off('data', handleAck);
                    resolve(true);
                }
            };

            conn.on('data', handleAck);
        });
    }
}