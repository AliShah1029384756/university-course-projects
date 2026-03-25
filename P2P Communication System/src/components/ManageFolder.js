import React, { useState, useEffect } from 'react';
import styled from 'styled-components';
import { FaFolder, FaFile } from 'react-icons/fa';

const ManageFolder = ({ selectedFolder }) => {
    const [folderContents, setFolderContents] = useState([]);
    const [folderName, setFolderName] = useState('');
    const [isLoading, setIsLoading] = useState(true);

    useEffect(() => {
        loadFolderContents();
    }, [selectedFolder]);

    const loadFolderContents = async () => {
        if (!selectedFolder) {
            setIsLoading(false);
            return;
        }

        try {
            setIsLoading(true);
            setFolderName(selectedFolder.name);
            
            // Get all files in the folder
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
            setFolderContents(files);
        } catch (error) {
            console.error('Error loading folder contents:', error);
        } finally {
            setIsLoading(false);
        }
    };

    if (!selectedFolder) {
        return (
            <Container>
                <NoFolderMessage>
                    <FaFolder size={48} color="#9CA3AF" />
                    <p>No folder selected. Please select a folder first.</p>
                </NoFolderMessage>
            </Container>
        );
    }

    return (
        <Container>
            <FolderHeader>
                <FolderIcon>
                    <FaFolder size={24} />
                </FolderIcon>
                <FolderName>{folderName}</FolderName>
                <FolderStats>
                    {folderContents.length} files
                </FolderStats>
            </FolderHeader>

            {isLoading ? (
                <LoadingMessage>Loading folder contents...</LoadingMessage>
            ) : (
                <FileList>
                    {folderContents.map((file, index) => (
                        <FileItem key={index}>
                            <FileIcon>
                                <FaFile />
                            </FileIcon>
                            <FileDetails>
                                <FileName>{file.name}</FileName>
                                <FileInfo>
                                    {(file.size / 1024).toFixed(2)} KB • 
                                    {new Date(file.lastModified).toLocaleDateString()}
                                </FileInfo>
                            </FileDetails>
                        </FileItem>
                    ))}
                    {folderContents.length === 0 && (
                        <EmptyFolder>
                            This folder is empty
                        </EmptyFolder>
                    )}
                </FileList>
            )}
        </Container>
    );
};

const Container = styled.div`
    max-width: 800px;
    margin: 0 auto;
    padding: 20px;
`;

const FolderHeader = styled.div`
    display: flex;
    align-items: center;
    padding: 20px;
    background: white;
    border-radius: 12px;
    box-shadow: 0 2px 4px rgba(0,0,0,0.1);
    margin-bottom: 20px;
`;

const FolderIcon = styled.div`
    color: #3f51b5;
    margin-right: 12px;
`;

const FolderName = styled.h2`
    margin: 0;
    color: #1f2937;
    font-size: 1.5em;
`;

const FolderStats = styled.div`
    margin-left: auto;
    color: #6b7280;
    font-size: 0.9em;
`;

const FileList = styled.div`
    background: white;
    border-radius: 12px;
    box-shadow: 0 2px 4px rgba(0,0,0,0.1);
    overflow: hidden;
`;

const FileItem = styled.div`
    display: flex;
    align-items: center;
    padding: 16px;
    border-bottom: 1px solid #e5e7eb;

    &:last-child {
        border-bottom: none;
    }

    &:hover {
        background: #f9fafb;
    }
`;

const FileIcon = styled.div`
    color: #6b7280;
    margin-right: 12px;
`;

const FileDetails = styled.div`
    flex: 1;
`;

const FileName = styled.div`
    color: #1f2937;
    font-weight: 500;
    margin-bottom: 4px;
`;

const FileInfo = styled.div`
    color: #6b7280;
    font-size: 0.85em;
`;

const NoFolderMessage = styled.div`
    text-align: center;
    padding: 40px;
    background: white;
    border-radius: 12px;
    box-shadow: 0 2px 4px rgba(0,0,0,0.1);
    color: #6b7280;

    p {
        margin-top: 16px;
    }
`;

const LoadingMessage = styled.div`
    text-align: center;
    padding: 20px;
    color: #6b7280;
`;

const EmptyFolder = styled.div`
    text-align: center;
    padding: 40px;
    color: #6b7280;
`;

export default ManageFolder; 