export const CHUNK_SIZE = 1024 * 1024; // 1MB chunks

export const splitFileIntoChunks = async (file) => {
    const chunks = [];
    const totalChunks = Math.ceil(file.size / CHUNK_SIZE);
    
    for (let i = 0; i < totalChunks; i++) {
        const start = i * CHUNK_SIZE;
        const end = Math.min(start + CHUNK_SIZE, file.size);
        chunks.push({
            index: i,
            data: file.slice(start, end),
            size: end - start,
            hash: await computeHash(file.slice(start, end))
        });
    }
    
    return {
        fileName: file.name,
        totalSize: file.size,
        totalChunks,
        chunks,
        metadata: {
            name: file.name,
            size: file.size,
            type: file.type,
            lastModified: file.lastModified
        }
    };
};

const computeHash = async (blob) => {
    const buffer = await blob.arrayBuffer();
    const hashBuffer = await crypto.subtle.digest('SHA-256', buffer);
    return Array.from(new Uint8Array(hashBuffer))
        .map(b => b.toString(16).padStart(2, '0'))
        .join('');
};

export const reassembleFile = async (chunks, metadata) => {
    // Sort chunks by index
    chunks.sort((a, b) => a.index - b.index);
    
    // Combine chunks into a single blob
    const blob = new Blob(chunks.map(chunk => chunk.data), { type: metadata.type });
    return new File([blob], metadata.name, {
        type: metadata.type,
        lastModified: metadata.lastModified
    });
}; 