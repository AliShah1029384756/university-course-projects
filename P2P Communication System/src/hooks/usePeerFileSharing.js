import { useState, useEffect } from 'react';

export const usePeerFileSharing = (peers, selectedFolder) => {
    const [peerFiles, setPeerFiles] = useState(new Map());
    const [localFiles, setLocalFiles] = useState([]);
    const [isLoading, setIsLoading] = useState(false);

    useEffect(() => {
        if (selectedFolder) {
            loadLocalFiles();
        }
    }, [selectedFolder]);

    const loadLocalFiles = async () => {
        if (!selectedFolder) return;
        try {
            const files = [];
            for await (const entry of selectedFolder.values()) {
                if (entry.kind === 'file') {
                    const file = await entry.getFile();
                    files.push({
                        name: file.name,
                        size: file.size,
                        type: file.type,
                        lastModified: file.lastModified
                    });
                }
            }
            setLocalFiles(files);
        } catch (error) {
            console.error('Error loading local files:', error);
        }
    };

    const requestPeerFiles = () => {
        setIsLoading(true);
        peers.forEach(conn => {
            if (conn.open) {
                conn.send({
                    type: 'request_peer_files',
                    isFileOperation: true // Flag to identify file operations
                });
            }
        });
        setTimeout(() => setIsLoading(false), 2000);
    };

    const handleFileRequest = async (fileName, peerId) => {
        const conn = peers.get(peerId);
        if (!conn || !conn.open) return;

        conn.send({
            type: 'peer_file_request',
            fileName,
            isFileOperation: true // Flag to identify file operations
        });
    };

    const handleIncomingFileOperation = (data, conn) => {
        if (!data.isFileOperation) return; // Only handle file operations

        switch (data.type) {
            case 'peer_file_list':
                setPeerFiles(prev => {
                    const newMap = new Map(prev);
                    newMap.set(conn.peer, data.files);
                    return newMap;
                });
                break;
            case 'request_peer_files':
                sendFileList(conn);
                break;
            case 'peer_file_request':
                sendRequestedFile(data.fileName, conn);
                break;
            case 'peer_file_data':
                handleFileDownload(data);
                break;
        }
    };

    const sendFileList = async (conn) => {
        if (!selectedFolder || !conn.open) return;
        try {
            const files = await loadLocalFiles();
            conn.send({
                type: 'peer_file_list',
                files,
                isFileOperation: true
            });
        } catch (error) {
            console.error('Error sending file list:', error);
        }
    };

    const sendRequestedFile = async (fileName, conn) => {
        if (!selectedFolder || !conn.open) return;
        try {
            for await (const entry of selectedFolder.values()) {
                if (entry.kind === 'file' && entry.name === fileName) {
                    const file = await entry.getFile();
                    const arrayBuffer = await file.arrayBuffer();
                    conn.send({
                        type: 'peer_file_data',
                        fileName: file.name,
                        fileType: file.type,
                        fileData: arrayBuffer,
                        isFileOperation: true
                    });
                    break;
                }
            }
        } catch (error) {
            console.error('Error sending file:', error);
        }
    };

    const handleFileDownload = (data) => {
        try {
            const blob = new Blob([data.fileData], { type: data.fileType });
            const url = URL.createObjectURL(blob);
            const a = document.createElement('a');
            a.href = url;
            a.download = data.fileName;
            document.body.appendChild(a);
            a.click();
            document.body.removeChild(a);
            URL.revokeObjectURL(url);
        } catch (error) {
            console.error('Error downloading file:', error);
        }
    };

    return {
        peerFiles,
        localFiles,
        isLoading,
        requestPeerFiles,
        handleFileRequest,
        handleIncomingFileOperation
    };
}; 