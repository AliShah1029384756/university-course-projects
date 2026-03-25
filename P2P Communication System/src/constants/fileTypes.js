export const FILE_TYPES = {
    START: 'file-transfer-start',
    CHUNK: 'file-transfer-chunk',
    CHUNK_ACK: 'file-transfer-chunk-ack',
    END: 'file-transfer-end',
    TRANSFER_COMPLETE: 'file-transfer-complete'
};

export const CHUNK_SIZE = 16384; // 16KB chunks
export const FILE_TRANSFER_TIMEOUT = 5000; // 5 seconds
export const MAX_RETRIES = 3;
export const CHUNK_DELAY = 50; // 50ms delay between chunks