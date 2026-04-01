import React, { useState, useEffect } from 'react';
import styled from 'styled-components';
import { FaFolder, FaFile, FaDownload } from 'react-icons/fa';

const FileSharing = ({ peerConnections, selectedFolder }) => {
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
        peerConnections.forEach(conn => {
            if (conn.open) {
                conn.send({
                    type: 'request-peer-files'
                });
            }
        });
        setTimeout(() => setIsLoading(false), 2000);
    };

    const handleFileRequest = (fileName, peerId) => {
        const conn = peerConnections.get(peerId);
        if (conn && conn.open) {
            conn.send({
                type: 'file-request',
                fileName
            });
        }
    };

    return (
        <Container>
            <Header>
                <h2>File Sharing</h2>
                <RefreshButton onClick={requestPeerFiles}>
                    Refresh Files
                </RefreshButton>
            </Header>

            <Section>
                <SectionTitle>My Files</SectionTitle>
                <FileList>
                    {localFiles.map((file, index) => (
                        <FileItem key={index}>
                            <FileIcon><FaFile /></FileIcon>
                            <FileDetails>
                                <FileName>{file.name}</FileName>
                                <FileSize>{(file.size / 1024).toFixed(2)} KB</FileSize>
                            </FileDetails>
                        </FileItem>
                    ))}
                    {localFiles.length === 0 && (
                        <EmptyMessage>No files in selected folder</EmptyMessage>
                    )}
                </FileList>
            </Section>

            <Section>
                <SectionTitle>Peer Files</SectionTitle>
                <PeerGrid>
                    {Array.from(peerConnections.entries()).map(([peerId, conn]) => (
                        <PeerSection key={peerId}>
                            <PeerHeader>
                                <FaFolder />
                                <span>Peer {peerId.slice(0, 5)}</span>
                                <OnlineStatus active={conn.open} />
                            </PeerHeader>
                            <FileList>
                                {peerFiles.get(peerId)?.map((file, index) => (
                                    <FileItem key={index}>
                                        <FileIcon><FaFile /></FileIcon>
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
                                    <EmptyMessage>No shared files</EmptyMessage>
                                )}
                            </FileList>
                        </PeerSection>
                    ))}
                </PeerGrid>
            </Section>
        </Container>
    );
};

// Styled components
const Container = styled.div`
    padding: 20px;
    max-width: 1200px;
    margin: 0 auto;
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

const Section = styled.div`
    margin-bottom: 32px;
`;

const SectionTitle = styled.h3`
    color: #4b5563;
    margin-bottom: 16px;
`;

const FileList = styled.div`
    background: white;
    border-radius: 12px;
    box-shadow: 0 2px 4px rgba(0,0,0,0.1);
    overflow: hidden;
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

const EmptyMessage = styled.div`
    text-align: center;
    padding: 20px;
    color: #6b7280;
`;

export default FileSharing; 