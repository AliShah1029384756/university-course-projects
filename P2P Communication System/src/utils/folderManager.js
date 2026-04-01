export const initializeP2PFolder = async () => {
    try {
        // Request permission to access files
        const dirHandle = await window.showDirectoryPicker({
            mode: 'readwrite',
            startIn: 'documents'
        });
        
        return dirHandle;
    } catch (error) {
        console.error('Error initializing folder:', error);
        return null;
    }
};

export const saveFileToFolder = async (dirHandle, file, content) => {
    try {
        const fileHandle = await dirHandle.getFileHandle(file.name, { create: true });
        const writable = await fileHandle.createWritable();
        await writable.write(content);
        await writable.close();
        return true;
    } catch (error) {
        console.error('Error saving file:', error);
        return false;
    }
};

export const getFilesFromFolder = async (dirHandle) => {
    try {
        const files = [];
        for await (const entry of dirHandle.values()) {
            if (entry.kind === 'file') {
                try {
                    const file = await entry.getFile();
                    files.push({
                        name: file.name,
                        size: file.size,
                        type: file.type,
                        lastModified: file.lastModified,
                        handle: entry
                    });
                } catch (error) {
                    console.error(`Error getting file ${entry.name}:`, error);
                }
            }
        }
        return files;
    } catch (error) {
        console.error('Error reading files from folder:', error);
        throw error;
    }
};

export const readFileFromFolder = async (dirHandle, fileName) => {
    try {
        const fileHandle = await dirHandle.getFileHandle(fileName);
        const file = await fileHandle.getFile();
        return await file.arrayBuffer();
    } catch (error) {
        console.error('Error reading file:', error);
        return null;
    }
}; 