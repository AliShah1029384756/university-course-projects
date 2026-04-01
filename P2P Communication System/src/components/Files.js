import React, { useState } from 'react';
import styled from 'styled-components';
import { FaFolder, FaFile, FaDownload } from 'react-icons/fa';

const Files = ({ connections, peerFiles, myPeerId, sendEncryptedData }) => {
    const [selectedPeer, setSelectedPeer] = useState(null);

    const handleFileRequest = (fileName, peerId) => {
        sendEncryptedData({
            type: 'file-request',
            fileName,
            requesterId: myPeerId
        }, peerId);
    };

    return (
        <Container>
            <Header>
                <h2>Shared Files</h2>
            </Header>

            <PeerGrid>
                {Array.from(connections.entries()).map(([peerId, conn]) => (
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
        </Container>
    );
};

const Container = styled.div`
    padding: 20px;
    max-width: 1200px;
    margin: 0 auto;
`;

const Header = styled.div`
    margin-bottom: 30px;
    text-align: center;

    h2 {
        margin: 0;
        color: #1f2937;
    }
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

const FileList = styled.div`
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

const EmptyMessage = styled.div`
    text-align: center;
    padding: 20px;
    color: #6b7280;
`;

export default Files; 