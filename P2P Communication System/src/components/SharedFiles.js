import React, { useState, useEffect } from 'react';
import styled from 'styled-components';
import { getFilesFromFolder } from '../utils/folderManager';

const SharedFiles = ({ selectedFolderHandle }) => {
    const [files, setFiles] = useState([]);
    const [loading, setLoading] = useState(true);

    useEffect(() => {
        loadFiles();
    }, [selectedFolderHandle]);

    const loadFiles = async () => {
        if (selectedFolderHandle) {
            try {
                setLoading(true);
                const folderFiles = await getFilesFromFolder(selectedFolderHandle);
                setFiles(folderFiles);
            } catch (error) {
                console.error('Error loading files:', error);
            } finally {
                setLoading(false);
            }
        }
    };

    return (
        <Container>
            <Header>
                <h3>Shared Files</h3>
            </Header>
            {loading ? (
                <LoadingState>Loading files...</LoadingState>
            ) : files.length === 0 ? (
                <EmptyState>
                    No files in shared folder
                </EmptyState>
            ) : (
                <FileList>
                    {files.map((file, index) => (
                        <FileItem key={index}>
                            <FileIcon>📄</FileIcon>
                            <FileDetails>
                                <FileName>{file.name}</FileName>
                                <FileSize>{(file.size / 1024).toFixed(2)} KB</FileSize>
                            </FileDetails>
                        </FileItem>
                    ))}
                </FileList>
            )}
        </Container>
    );
};

const Container = styled.div`
    padding: 20px;
    background: white;
    border-radius: 12px;
    box-shadow: 0 2px 8px rgba(0,0,0,0.05);
    margin-bottom: 20px;
`;

const Header = styled.div`
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 20px;

    h3 {
        margin: 0;
        color: #1f2937;
    }
`;

const FileList = styled.div`
    margin-top: 20px;
`;

const FileItem = styled.div`
    display: flex;
    align-items: center;
    padding: 12px;
    border: 1px solid #e5e7eb;
    border-radius: 8px;
    margin-bottom: 8px;
    background: #f9fafb;
    transition: all 0.2s;

    &:hover {
        background: #f3f4f6;
        transform: translateX(2px);
    }
`;

const FileIcon = styled.span`
    font-size: 1.5em;
    margin-right: 12px;
    color: #6b7280;
`;

const FileDetails = styled.div`
    flex: 1;
`;

const FileName = styled.div`
    font-weight: 500;
    color: #1f2937;
    margin-bottom: 4px;
`;

const FileSize = styled.div`
    font-size: 0.8em;
    color: #6b7280;
`;

const EmptyState = styled.div`
    text-align: center;
    padding: 40px;
    color: #6b7280;
    background: #f8f9fa;
    border-radius: 8px;
`;

const LoadingState = styled.div`
    text-align: center;
    padding: 40px;
    color: #6b7280;
`;

export default SharedFiles; 