import React, { useState } from 'react';
import styled from 'styled-components';

const FolderSelector = ({ onFolderSelect }) => {
    const [selectedFolder, setSelectedFolder] = useState(null);

    const handleFolderSelect = async () => {
        try {
            const dirHandle = await window.showDirectoryPicker();
            setSelectedFolder(dirHandle);
            onFolderSelect(dirHandle);
        } catch (error) {
            console.error('Error selecting folder:', error);
        }
    };

    return (
        <Container>
            <Button onClick={handleFolderSelect}>
                {selectedFolder ? 'Change Folder' : 'Select Shared Folder'}
            </Button>
            {selectedFolder && (
                <FolderInfo>
                    Selected: {selectedFolder.name}
                </FolderInfo>
            )}
        </Container>
    );
};

const Container = styled.div`
    margin-bottom: 20px;
`;

const Button = styled.button`
    padding: 10px 20px;
    background: #3f51b5;
    color: white;
    border: none;
    border-radius: 8px;
    cursor: pointer;
    font-weight: 500;
    transition: all 0.2s;

    &:hover {
        background: #303f9f;
    }
`;

const FolderInfo = styled.div`
    margin-top: 10px;
    color: #666;
    font-size: 0.9em;
`;

export default FolderSelector; 