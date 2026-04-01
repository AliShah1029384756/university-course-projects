import CryptoJS from 'crypto-js';

// Using a constant encryption key for development
// In production, this should come from environment variables
const ENCRYPTION_KEY = 'P2P-CHAT-APP-SECRET-KEY-2024';

export const encryptMessage = (message) => {
    try {
        return CryptoJS.AES.encrypt(JSON.stringify(message), ENCRYPTION_KEY).toString();
    } catch (error) {
        console.error('Encryption error:', error);
        throw error;
    }
};

export const decryptMessage = (encryptedMessage) => {
    try {
        const bytes = CryptoJS.AES.decrypt(encryptedMessage, ENCRYPTION_KEY);
        return JSON.parse(bytes.toString(CryptoJS.enc.Utf8));
    } catch (error) {
        console.error('Decryption error:', error);
        throw error;
    }
}; 