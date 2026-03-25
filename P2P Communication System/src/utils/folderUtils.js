export const selectFolder = async () => {
    try {
        // Create a file input element
        const input = document.createElement('input');
        input.type = 'file';
        input.webkitdirectory = true; // Allow directory selection
        input.directory = true;
        input.multiple = true; // Allow multiple files

        return new Promise((resolve, reject) => {
            input.onchange = (e) => {
                const files = Array.from(e.target.files);
                if (files.length > 0) {
                    // Get folder name from the first file's path
                    const folderName = files[0].webkitRelativePath.split('/')[0];
                    
                    // Create a folder-like object that matches what we need
                    const folderHandle = {
                        name: folderName,
                        files: files,
                        kind: 'directory',
                        isDirectory: true,
                        // Add methods we use
                        async getFile() {
                            return files[0];
                        },
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
            
            // Trigger the file input click
            input.click();
        });
    } catch (error) {
        throw new Error('Failed to select folder: ' + error.message);
    }
}; 