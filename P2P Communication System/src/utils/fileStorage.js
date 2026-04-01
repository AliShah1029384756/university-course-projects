let db = null;

const initDB = () => {
    return new Promise((resolve, reject) => {
        // First, try to delete the existing database
        const deleteRequest = indexedDB.deleteDatabase('p2p-files');
        
        deleteRequest.onsuccess = () => {
            console.log('Old database deleted successfully');
            // Create new database
            const request = indexedDB.open('p2p-files', 1);
            
            request.onerror = (event) => {
                console.error('Database error:', event.target.error);
                reject(request.error);
            };

            request.onupgradeneeded = (event) => {
                console.log('Creating new database...');
                const database = event.target.result;
                
                if (!database.objectStoreNames.contains('files')) {
                    database.createObjectStore('files', { keyPath: 'name' });
                    console.log('Files store created');
                }
            };

            request.onsuccess = (event) => {
                db = event.target.result;
                console.log('Database initialized successfully');
                resolve(db);
            };
        };

        deleteRequest.onerror = () => {
            console.error('Error deleting old database');
            reject(deleteRequest.error);
        };
    });
};

export const saveToLocalStorage = async (file, content) => {
    try {
        if (!db) {
            await initDB();
        }
        
        return new Promise((resolve, reject) => {
            const transaction = db.transaction(['files'], 'readwrite');
            const store = transaction.objectStore('files');

            const fileData = {
                name: file.name,
                type: file.type || 'application/octet-stream',
                size: file.size,
                content: content,
                timestamp: Date.now()
            };

            const request = store.put(fileData);
            
            request.onsuccess = () => {
                console.log('File saved successfully:', file.name);
                resolve(fileData);
            };
            
            request.onerror = () => {
                console.error('Error saving file:', request.error);
                reject(request.error);
            };
        });
    } catch (error) {
        console.error('Error in saveToLocalStorage:', error);
        throw error;
    }
};

export const getLocalFiles = async () => {
    try {
        if (!db) {
            await initDB();
        }
        
        return new Promise((resolve, reject) => {
            const transaction = db.transaction(['files'], 'readonly');
            const store = transaction.objectStore('files');
            const request = store.getAll();

            request.onsuccess = () => {
                console.log('Retrieved files:', request.result);
                resolve(request.result);
            };
            
            request.onerror = () => {
                console.error('Error getting files:', request.error);
                reject(request.error);
            };
        });
    } catch (error) {
        console.error('Error in getLocalFiles:', error);
        return [];
    }
};

export const deleteFile = async (fileName) => {
    try {
        if (!db) {
            await initDB();
        }
        
        return new Promise((resolve, reject) => {
            const transaction = db.transaction(['files'], 'readwrite');
            const store = transaction.objectStore('files');
            const request = store.delete(fileName);

            request.onsuccess = () => {
                console.log('File deleted successfully:', fileName);
                resolve(true);
            };
            
            request.onerror = () => {
                console.error('Error deleting file:', request.error);
                reject(request.error);
            };
        });
    } catch (error) {
        console.error('Error in deleteFile:', error);
        throw error;
    }
}; 