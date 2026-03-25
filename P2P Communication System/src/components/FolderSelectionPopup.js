import React, { useState } from 'react';
import styled from 'styled-components';

const FolderSelectionPopup = ({ onSelect }) => {
    const [error, setError] = useState('');
    const [isSelecting, setIsSelecting] = useState(false);

    const handleFolderSelect = async () => {
        if (isSelecting) return;
        setIsSelecting(true);
        setError('');

        try {
            const input = document.createElement('input');
            input.type = 'file';
            input.webkitdirectory = true;
            input.directory = true;
            input.multiple = true;

            const folderSelection = new Promise((resolve, reject) => {
                input.onchange = (e) => {
                    const files = Array.from(e.target.files);
                    if (files.length > 0) {
                        const folderName = files[0].webkitRelativePath.split('/')[0];
                        console.log('Selected folder name:', folderName);
                        console.log('Number of files:', files.length);
                        
                        const folderHandle = {
                            name: folderName,
                            files: files,
                            kind: 'directory',
                            isDirectory: true,
                            async *values() {
                                for (const file of files) {
                                    yield {
                                        kind: 'file',
                                        name: file.name,
                                        async getFile() {
                                            return file;
                                        }
                                    };
                                }
                            }
                        };
                        resolve(folderHandle);
                    } else {
                        reject(new Error('No folder selected'));
                    }
                };

                input.onerror = (error) => {
                    reject(error);
                };

                input.click();
            });

            const folderHandle = await folderSelection;
            console.log('Folder selection successful:', folderHandle.name);
            onSelect(folderHandle);
        } catch (error) {
            console.error('Error in folder selection:', error);
            if (error.name !== 'AbortError') {
                setError(`Error selecting folder: ${error.message}`);
            }
        } finally {
            setIsSelecting(false);
        }
    };

    return (
        <PopupOverlay>
            <PopupContent>
                <h2>Select Folder</h2>
                <p>Please select a folder to share and download files</p>
                {error && <ErrorMessage>{error}</ErrorMessage>}
                <SelectButton 
                    onClick={handleFolderSelect}
                    disabled={isSelecting}
                >
                    {isSelecting ? 'Selecting...' : 'Choose Folder'}
                </SelectButton>
            </PopupContent>
        </PopupOverlay>
    );
};

const PopupOverlay = styled.div`
    position: fixed;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background: rgba(0, 0, 0, 0.5);
    display: flex;
    align-items: center;
    justify-content: center;
    z-index: 1100;
`;

const PopupContent = styled.div`
    background: white;
    padding: 2rem;
    border-radius: 12px;
    text-align: center;
    max-width: 400px;
    width: 90%;

    h2 {
        margin: 0 0 1rem;
        color: #1f2937;
    }

    p {
        color: #6b7280;
        margin-bottom: 1.5rem;
    }
`;

const SelectButton = styled.button`
    padding: 12px 24px;
    background: ${props => props.disabled ? '#9fa8da' : '#3f51b5'};
    color: white;
    border: none;
    border-radius: 25px;
    font-weight: 500;
    cursor: ${props => props.disabled ? 'not-allowed' : 'pointer'};
    transition: all 0.2s;

    &:hover {
        background: ${props => props.disabled ? '#9fa8da' : '#303f9f'};
        transform: ${props => props.disabled ? 'none' : 'translateY(-1px)'};
    }
`;

const ErrorMessage = styled.div`
    color: #ef4444;
    background: #fee2e2;
    padding: 10px;
    border-radius: 4px;
    margin: 10px 0;
    font-size: 0.9em;
`;

export default FolderSelectionPopup; 