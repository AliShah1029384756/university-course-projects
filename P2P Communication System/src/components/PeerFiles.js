import React, { useState, useEffect } from 'react';
import styled from 'styled-components';
import { FaFolder, FaFile, FaDownload } from 'react-icons/fa';

const PeerFiles = ({ peers, peerInstance, selectedFolder }) => {
    const [peerFiles, setPeerFiles] = useState(new Map());
    const [isLoading, setIsLoading] = useState(true);

    useEffect(() => {
        // Request files list from all peers when component mounts
        requestFilesFromPeers();
        
        // Set up listener for file list updates
        const handleFileList = (data, peerId) => {
            if (data.type === 'files-list') {
                setPeerFiles(prev => {
                    const newMap = new Map(prev);
                    newMap.set(peerId, data.files);
                    return newMap;
                });
            }
        };

        // Add event listeners to existing peer connections
        peers.forEach((conn, peerId) => {
            conn.on('data', (data) => handleFileList(data, peerId));
        });

        return () => {
            // Clean up listeners
            peers.forEach(conn => {
                conn.off('data');
            });
        };
    }, [peers]);

    const requestFilesFromPeers = () => {
        peers.forEach((conn) => {
            if (conn.open) {
                conn.send({
                    type: 'request-files'
                });
            }
        });
        setIsLoading(false);
    };

    const handleFileRequest = async (fileName, peerId) => {
        const conn = peers.get(peerId);
        if (conn && conn.open) {
            conn.send({
                type: 'file-request',
                fileName: fileName
            });
        }
    };

    if (isLoading) {
        return <LoadingMessage>Loading peer files...</LoadingMessage>;
    }

    return (
        <Container>
            <Header>
                <h2>Peer Files</h2>
                <RefreshButton onClick={requestFilesFromPeers}>
                    Refresh Files
                </RefreshButton>
            </Header>

            <PeerGrid>
                {Array.from(peers.entries()).map(([peerId, conn]) => (
                    <PeerSection key={peerId}>
                        <PeerHeader>
                            <FaFolder />
                            <span>Peer {peerId.slice(0, 5)}</span>
                            <OnlineStatus active={conn.open} />
                        </PeerHeader>
                        <FilesList>
                            {peerFiles.get(peerId)?.map((file, index) => (
                                <FileItem key={index}>
                                    <FileIcon>
                                        <FaFile />
                                    </FileIcon>
                                    <FileDetails>
                                        <FileName>{file.name}</FileName>
                                        <FileSize>
                                            {(file.size / 1024).toFixed(2)} KB
                                        </FileSize>
                                    </FileDetails>
                                    <DownloadButton
                                        onClick={() => handleFileRequest(file.name, peerId)}
                                    >
                                        <FaDownload />
                                    </DownloadButton>
                                </FileItem>
                            ))}
                            {(!peerFiles.get(peerId) || peerFiles.get(peerId).length === 0) && (
                                <EmptyMessage>No files shared</EmptyMessage>
                            )}
                        </FilesList>
                    </PeerSection>
                ))}
            </PeerGrid>
        </Container>
    );
};

// Styled components
const Container = styled.div`
    padding: 20px;
`;

const Header = styled.div`
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 20px;
`;

const RefreshButton = styled.button`
    padding: 8px 16px;
    background: #3f51b5;
    color: white;
    border: none;
    border-radius: 8px;
    cursor: pointer;
`;

const PeerGrid = styled.div`
    display: grid;
    grid-template-columns: repeat(auto-fill, minmax(300px, 1fr));
    gap: 20px;
`;

const PeerSection = styled.div`
    background: white;
    border-radius: 12px;
    box-shadow: 0 2px 4px rgba(0,0,0,0.1);
    overflow: hidden;
`;

const PeerHeader = styled.div`
    display: flex;
    align-items: center;
    padding: 16px;
    background: #f8f9fa;
    border-bottom: 1px solid #e5e7eb;
    gap: 12px;
`;

const OnlineStatus = styled.span`
    width: 8px;
    height: 8px;
    border-radius: 50%;
    background: ${props => props.active ? '#10b981' : '#ef4444'};
    margin-left: auto;
`;

const FilesList = styled.div`
    max-height: 300px;
    overflow-y: auto;
`;

const FileItem = styled.div`
    display: flex;
    align-items: center;
    padding: 12px 16px;
    border-bottom: 1px solid #e5e7eb;
    gap: 12px;

    &:hover {
        background: #f9fafb;
    }
`;

const FileIcon = styled.div`
    color: #6b7280;
`;

const FileDetails = styled.div`
    flex: 1;
`;

const FileName = styled.div`
    font-weight: 500;
    color: #1f2937;
`;

const FileSize = styled.div`
    font-size: 0.85em;
    color: #6b7280;
`;

const DownloadButton = styled.button`
    background: none;
    border: none;
    color: #3f51b5;
    cursor: pointer;
    padding: 8px;
    border-radius: 50%;
    transition: all 0.2s;

    &:hover {
        background: #f3f4f6;
    }
`;

const LoadingMessage = styled.div`
    text-align: center;
    padding: 40px;
    color: #6b7280;
`;

const EmptyMessage = styled.div`
    text-align: center;
    padding: 20px;
    color: #6b7280;
`;

export default PeerFiles; 