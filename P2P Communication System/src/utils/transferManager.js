import { reassembleFile } from './fileChunker';

export class TransferManager {
    constructor() {
        this.activeTransfers = new Map(); // transferId -> transfer state
        this.chunkCache = new Map(); // fileHash -> chunks
        this.peerFiles = new Map(); // peerId -> available files
    }

    startTransfer(fileInfo, peers) {
        const transferId = `${fileInfo.fileName}-${Date.now()}`;
        const transfer = {
            id: transferId,
            fileInfo,
            missingChunks: Array.from({ length: fileInfo.totalChunks }, (_, i) => i),
            receivedChunks: new Map(),
            availablePeers: peers,
            status: 'downloading',
            progress: 0
        };
        
        this.activeTransfers.set(transferId, transfer);
        this.requestNextChunks(transfer);
        return transferId;
    }

    handleChunkReceived(transferId, chunk, fromPeerId) {
        const transfer = this.activeTransfers.get(transferId);
        if (!transfer) return;

        // Verify chunk hash
        const expectedChunk = transfer.fileInfo.chunks[chunk.index];
        if (expectedChunk.hash !== chunk.hash) {
            console.error('Invalid chunk received');
            return;
        }

        // Store chunk
        transfer.receivedChunks.set(chunk.index, chunk);
        transfer.missingChunks = transfer.missingChunks.filter(i => i !== chunk.index);
        
        // Update progress
        transfer.progress = (transfer.receivedChunks.size / transfer.fileInfo.totalChunks) * 100;

        // Request more chunks if needed
        if (transfer.missingChunks.length > 0) {
            this.requestNextChunks(transfer);
        } else {
            this.completeTransfer(transfer);
        }
    }

    requestNextChunks(transfer) {
        const PARALLEL_REQUESTS = 5;
        const chunksToRequest = transfer.missingChunks.slice(0, PARALLEL_REQUESTS);
        
        chunksToRequest.forEach(chunkIndex => {
            // Select random peer that has the file
            const availablePeers = Array.from(transfer.availablePeers);
            const peer = availablePeers[Math.floor(Math.random() * availablePeers.length)];
            
            this.requestChunkFromPeer(transfer.id, chunkIndex, peer);
        });
    }

    async completeTransfer(transfer) {
        const chunks = Array.from(transfer.receivedChunks.values());
        const file = await reassembleFile(chunks, transfer.fileInfo.metadata);
        
        transfer.status = 'completed';
        transfer.file = file;
        
        // Notify completion
        if (transfer.onComplete) {
            transfer.onComplete(file);
        }
    }
} 