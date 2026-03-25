export const getFilesFromFolder = async (folderHandle) => {
    if (!folderHandle) return [];
    
    try {
        const files = [];
        for await (const entry of folderHandle.values()) {
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
        return files;
    } catch (error) {
        console.error('Error getting files from folder:', error);
        return [];
    }
}; 