import { useEffect, useState, useRef, useCallback } from 'react';
import Peer from 'peerjs';
import styled from 'styled-components';
import { FaUsers, FaSearch, FaCog, FaUserCircle, FaHome, FaFolder, FaFileAlt, FaTrash, FaFile } from 'react-icons/fa';
import { saveToLocalStorage, getLocalFiles } from './utils/fileStorage';
import SharedFiles from './components/SharedFiles';
import { initializeP2PFolder, saveFileToFolder, getFilesFromFolder, readFileFromFolder } from './utils/folderManager';
import FolderSelectionPopup from './components/FolderSelectionPopup';
import ManageFolder from './components/ManageFolder';
import Files from './components/Files';
import PeerFiles from './components/PeerFiles';
import PeerFileSharing from './components/PeerFileSharing';
import { usePeerFileSharing } from './hooks/usePeerFileSharing';
import FileSharing from './components/FileSharing';
import PeerFileRequest from './components/PeerFileRequest';
import { FileTransferManager } from './utils/fileTransferManager';
import { FILE_TYPES } from './constants/fileTypes';

const sentMessages = new Set();

// Constants
const PEER_FILE_CHECK_INTERVAL = 60000;    // 60 seconds
const FILE_TRANSFER_COOLDOWN = 30000;      // 30 seconds
const INITIAL_CHECK_DELAY = 10000;         // 10 seconds
const CONNECTION_RETRY_DELAY = 5000;       // 5 seconds
const PEER_FILE_REQUEST_DELAY = 15000;     // 15 seconds
const CHUNK_SIZE = 32768;                  // 32KB chunks

// Helper functions
const formatFileSize = (bytes) => {
    if (bytes === 0) return '0 Bytes';
    const k = 1024;
    const sizes = ['Bytes', 'KB', 'MB', 'GB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
};

// Styled components
const AppContainer = styled.div`
    min-height: 100vh;
    background: #1a1a1a;
`;

const ChatContainer = styled.div`
    max-width: 1200px;
    margin: 0 auto;
    padding: 20px;
`;

const GroupInfo = styled.div`
    text-align: center;
    margin-bottom: 20px;
    padding: 12px;
    background: #2a2a2a;
    border-radius: 12px;
    box-shadow: 0 2px 8px rgba(0,0,0,0.2);
`;

const GroupName = styled.h2`
    margin: 0;
    color: #ff5722;
    font-size: 1.5em;
    font-weight: 600;
`;

const PeerCount = styled.div`
    color: #b0b0b0;
    font-size: 0.9em;
    margin-top: 4px;
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 4px;
`;

const OnlineIndicator = styled.span`
    display: inline-block;
    width: 8px;
    height: 8px;
    border-radius: 50%;
    background: ${props => props.active ? '#4caf50' : '#f44336'};
    margin-right: 8px;
`;

const StatusBar = styled.div`
    text-align: center;
    color: #b0b0b0;
    margin-bottom: 24px;
    padding: 8px;
    background: #2a2a2a;
    border-radius: 8px;
    font-size: 0.9em;
    box-shadow: 0 2px 8px rgba(0,0,0,0.2);
`;

const ChatSection = styled.div`
    border-radius: 16px;
    overflow: hidden;
    background: #2a2a2a;
    box-shadow: 0 4px 20px rgba(0, 0, 0, 0.2);
    display: flex;
    flex-direction: column;
    height: 80vh;
`;

const MessagesContainer = styled.div`
    flex: 1;
    overflow-y: auto;
    padding: 24px;
    background: #1a1a1a;
    scroll-behavior: smooth;
`;

const Message = styled.div`
    margin: 12px 0;
    max-width: 65%;
    ${props => props.sender === 'local' ? 'margin-left: auto;' : 'margin-right: auto;'}
`;

const MessageHeader = styled.div`
    display: flex;
    justify-content: space-between;
    align-items: center;
`;

const SenderName = styled.div`
    font-size: 0.85em;
    margin-bottom: 4px;
    color: #b0b0b0;
    font-weight: 500;
`;

const DeleteButton = styled.button`
    background: none;
    border: none;
    cursor: pointer;
    opacity: 0.6;
    padding: 4px;
    border-radius: 50%;
    transition: all 0.2s;
    &:hover {
        opacity: 1;
        background: #3a3a3a;
    }
`;

const FileMessage = styled.div`
    display: flex;
    align-items: center;
    gap: 12px;
    background: #2a2a2a;
    padding: 12px;
    border-radius: 12px;
    border: 1px solid #3a3a3a;
`;

const FileIcon = styled.span`
    font-size: 1.5em;
    color: #b0b0b0;
`;

const FileDetails = styled.div`
    flex: 1;
`;

const DownloadButton = styled.button`
    padding: 6px 12px;
    background: #ff5722;
    color: #131414;
    border: none;
    border-radius: 16px;
    cursor: pointer;
    font-weight: 500;
    font-size: 0.9em;
`;

const MessageText = styled.div`
    padding: 12px 18px;
    border-radius: 18px;
    font-size: 0.95em;
    line-height: 1.4;
    ${props => props.sender === 'local' ? `
        background: #ff5722;
        color: #131414;
        border-bottom-right-radius: 4px;
    ` : `
        background: #2a2a2a;
        color: #e0e0e0;
        border-bottom-left-radius: 4px;
    `}
    box-shadow: 0 1px 2px rgba(0,0,0,0.2);
`;

const InputSection = styled.div`
    padding: 12px;
    background: #1a1a1a;
    border-top: 1px solid #3a3a3a;
`;

const MessageInputWrapper = styled.div`
    display: flex;
    gap: 12px;
    align-items: center;
    background: #2a2a2a;
    padding: 8px 16px;
    border-radius: 30px;
    border: 1px solid #3a3a3a;
`;

const QuickEmojis = styled.div`
    display: flex;
    gap: 4px;
`;

const EmojiButton = styled.button`
    background: transparent;
    border: none;
    cursor: pointer;
    font-size: 1.4em;
    padding: 4px;
    border-radius: 50%;
    transition: all 0.2s;
    &:hover {
        background: #3a3a3a;
        transform: scale(1.1);
    }
`;

const MessageInput = styled.input`
    flex: 1;
    padding: 12px;
    border: none;
    background: transparent;
    font-size: 1em;
    color: #e0e0e0;
    &:focus {
        outline: none;
    }
`;

const ActionButtons = styled.div`
    display: flex;
    gap: 8px;
`;

const FileInput = styled.input.attrs({ type: 'file' })`
    display: none;
`;

const ActionButton = styled.button`
    width: 36px;
    height: 36px;
    border-radius: 50%;
    border: none;
    background: #ff5722;
    color: #131414;
    font-size: 1.2em;
    display: flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
    transition: all 0.2s;
    &:hover {
        background: #d84315;
        transform: scale(1.05);
    }
`;

const HomeContainer = styled.div`
    min-height: calc(100vh - 70px);
`;

const HeroSection = styled.section`
    height: 70vh;
    background: linear-gradient(135deg, #ff5722 0%, #d84315 100%);
    display: flex;
    align-items: center;
    justify-content: center;
    color: #e0e0e0;
    text-align: center;
    padding: 2rem;
`;

const HeroContent = styled.div`
    h1 {
        font-size: 3rem;
        margin-bottom: 1rem;
    }
    p {
        font-size: 1.2rem;
        margin-bottom: 2rem;
    }
`;

const StartButton = styled.button`
    padding: 1rem 2rem;
    font-size: 1.2rem;
    background: #ff5722;
    color: #131414;
    border: none;
    border-radius: 30px;
    cursor: pointer;
    transition: all 0.2s;
    &:hover {
        transform: translateY(-2px);
        box-shadow: 0 4px 12px rgba(0,0,0,0.4);
    }
`;

const FeaturesSection = styled.section`
    padding: 4rem 2rem;
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
    gap: 2rem;
    background: #1a1a1a;
`;

const FeatureCard = styled.div`
    text-align: center;
    padding: 2rem;
    background: #2a2a2a;
    border-radius: 12px;
    transition: all 0.2s;
    h3 {
        margin: 1rem 0;
        color: #ff5722;
    }
    &:hover {
        transform: translateY(-5px);
        box-shadow: 0 4px 12px rgba(0,0,0,0.2);
    }
`;

const Navbar = styled.nav`
    display: flex;
    align-items: center;
    padding: 1rem 2rem;
    background: #2a2a2a;
    box-shadow: 0 2px 4px rgba(0,0,0,0.2);
    position: fixed;
    top: 0;
    left: 0;
    right: 0;
    z-index: 1000;
`;

const NavLogo = styled.div`
    font-size: 1.5rem;
    font-weight: bold;
    color: #ff5722;
    display: flex;
    align-items: center;
    gap: 8px;
    cursor: pointer;
`;

const NavLinks = styled.div`
    display: flex;
    gap: 2rem;
    margin-left: auto;
`;

const NavItem = styled.button`
    background: none;
    border: none;
    padding: 0.5rem 1rem;
    color: ${props => props.active ? '#ff5722' : '#b0b0b0'};
    font-weight: ${props => props.active ? '600' : '400'};
    display: flex;
    align-items: center;
    gap: 6px;
    cursor: pointer;
    transition: all 0.2s;
    &:hover {
        color: #ff5722;
    }
`;

const SearchBar = styled.div`
    position: absolute;
    top: 100%;
    left: 0;
    right: 0;
    padding: 1rem;
    background: #2a2a2a;
    box-shadow: 0 2px 4px rgba(0,0,0,0.2);
`;

const SearchInput = styled.input`
    width: 100%;
    padding: 0.75rem;
    border: 1px solid #3a3a3a;
    border-radius: 8px;
    font-size: 1em;
    margin-bottom: 10px;
    &:focus {
        outline: none;
        border-color: #ff5722;
    }
`;

const SearchResults = styled.div`
    max-height: 300px;
    overflow-y: auto;
    border-top: 1px solid #3a3a3a;
`;

const SearchResult = styled.div`
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 10px;
    border-bottom: 1px solid #3a3a3a;
`;

const SearchingIndicator = styled.div`
    text-align: center;
    color: #b0b0b0;
    padding: 10px;
`;

const NoResults = styled.div`
    text-align: center;
    color: #b0b0b0;
    padding: 20px;
`;

const MainContent = styled.main`
    padding-top: 70px;
`;

const EmptyState = styled.div`
    text-align: center;
    padding: 2rem;
    color: #b0b0b0;
    background: #2a2a2a;
    border-radius: 8px;
    box-shadow: 0 2px 4px rgba(0,0,0,0.2);
`;

const RequestButton = styled.button`
    padding: 6px 12px;
    background: #ff5722;
    color: #131414;
    border: none;
    border-radius: 16px;
    cursor: pointer;
    font-size: 0.9em;
    &:hover {
        background: #d84315;
    }
`;

const FileInfo = styled.div`
    flex: 1;
    display: flex;
    flex-direction: column;
    gap: 4px;
`;

const FileName = styled.div`
    font-weight: 500;
    color: #e0e0e0;
`;

const FileSize = styled.div`
    font-size: 0.8em;
    color: #b0b0b0;
`;

const PeerInfo = styled.div`
    font-size: 0.8em;
    color: #b0b0b0;
    margin-right: 12px;
`;

const PeerFilesContainer = styled.div`
    padding: 20px;
    max-width: 800px;
    margin: 0 auto;
`;

const PeerSection = styled.div`
    background: #2a2a2a;
    border-radius: 8px;
    margin-bottom: 20px;
    box-shadow: 0 2px 4px rgba(0,0,0,0.2);
    overflow: hidden;
`;

const PeerHeader = styled.div`
    background: #2a2a2a;
    padding: 12px 20px;
    border-bottom: 1px solid #3a3a3a;
    
    h3 {
        margin: 0;
        color: #e0e0e0;
    }
`;

const FileList = styled.div`
    padding: 10px;
`;

const FileItem = styled.div`
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 10px;
    border-bottom: 1px solid #3a3a3a;
    
    &:last-child {
        border-bottom: none;
    }
`;

const RequestFileButton = styled.button`
    padding: 6px 12px;
    background: #ff5722;
    color: #131414;
    border: none;
    border-radius: 16px;
    cursor: pointer;
    font-size: 0.9em;
    
    &:hover {
        background: #d84315;
    }
`;

function App() {
    // Core chat states
    const [messages, setMessages] = useState([]);
    const [message, setMessage] = useState('');
    const [peers, setPeers] = useState(new Map());
    const [status, setStatus] = useState('Connecting...');
    const [currentPage, setCurrentPage] = useState('home');
    const [showEmojiPicker, setShowEmojiPicker] = useState(false);
    const [activeTab, setActiveTab] = useState('chat');
    
    // File sharing states
    const [localFiles, setLocalFiles] = useState([]);
    const [peerFiles, setPeerFiles] = useState(new Map());
    const [sharedFolderHandle, setSharedFolderHandle] = useState(null);
    const [selectedFolder, setSelectedFolder] = useState(null);
    const [showFolderPopup, setShowFolderPopup] = useState(true);
    const [sharedFiles, setSharedFiles] = useState(new Map());
    const [fileTransfers, setFileTransfers] = useState(new Map());
    const [incomingFiles, setIncomingFiles] = useState(new Map());
    const [peerFilesList, setPeerFilesList] = useState(new Map());
    const [isFileTransferInProgress, setIsFileTransferInProgress] = useState(false);
    
    // Search states
    const [showSearch, setShowSearch] = useState(false);
    const [searchQuery, setSearchQuery] = useState('');
    const [searchResults, setSearchResults] = useState([]);
    const [isSearching, setIsSearching] = useState(false);
    
    // Connection states
    const [manualPeerID, setManualPeerID] = useState('');
    const [isConnecting, setIsConnecting] = useState(false);
    
    // Refs
    const peerInstance = useRef(null);
    const fileInputRef = useRef(null);
    const fileTransferManagerRef = useRef(new FileTransferManager());
    const peerFileCheckIntervalRef = useRef(null);
    const chatConnectionsRef = useRef(new Map());
    const fileConnectionsRef = useRef(new Map());
    const messagesEndRef = useRef(null);
    const isFileTabActive = useRef(false);
    const lastPeerFileRequestTime = useRef({});
    const isReconnecting = useRef(false);

    const {
        handleIncomingFileOperation
    } = usePeerFileSharing(peers, selectedFolder);

    // Add this function for smooth scrolling
    const scrollToBottom = () => {
        messagesEndRef.current?.scrollIntoView({ behavior: "smooth" });
    };

    // Add this useEffect to scroll when new messages arrive
    useEffect(() => {
        scrollToBottom();
    }, [messages]);

    // Initialize peer connection
    useEffect(() => {
        const peer = new Peer(undefined, {
            host: 'localhost',
            port: 3001,
            path: '/peerjs/myapp',
            debug: 2,
        });

        peer.on('open', (id) => {
            setStatus(`Connected as: ${id}`);
            console.log('My peer ID is:', id);
            
            // Get list of all peers and connect to them
            peer.listAllPeers((peers) => {
                console.log('Available peers:', peers);
                peers.forEach(peerId => {
                    if (peerId !== id) {
                        console.log('Attempting to connect to peer:', peerId);
                        connectToPeer(peerId, peer);
                    }
                });
            });
        });

        peer.on('connection', (conn) => {
            console.log('Incoming connection from:', conn.peer);
            if (conn.metadata?.type === 'chat') {
                // Handle chat connection
                conn.on('open', () => {
                    console.log('Incoming chat connection opened with:', conn.peer);
                    handleChatConnection(conn);
                    
                    // Share our files with the new peer if we have a folder selected
                    if (selectedFolder) {
                        console.log('Sharing our files with new peer:', conn.peer);
                        setTimeout(() => {
                            sharePeerFiles();
                        }, 1000); // Add slight delay to ensure connection is ready
                    }
                });
            } else if (conn.metadata?.type === 'file') {
                // Handle file connection
                conn.on('open', () => {
                    console.log('Incoming file connection opened with:', conn.peer);
                    handleFileConnection(conn);
                });
            }
        });

        peerInstance.current = peer;

        return () => {
            peers.forEach(conn => conn.close());
            peer.destroy();
        };
    }, []);

    // Connect to a peer
    const connectToPeer = (peerId, peer) => {
        console.log('Creating connections to peer:', peerId);
        
        // Create chat connection
        const chatConn = peer.connect(peerId, {
            reliable: true,
            serialization: 'json',
            metadata: { type: 'chat' }
        });

        // Create separate file connection
        const fileConn = peer.connect(peerId, {
            reliable: true,
            serialization: 'binary',
            metadata: { type: 'file' }
        });

        // Handle chat connection first
        chatConn.on('open', () => {
            console.log('Chat connection opened with:', peerId);
            handleChatConnection(chatConn);
            
            // After chat connection is established, handle file connection
            fileConn.on('open', () => {
                console.log('File connection opened with:', peerId);
                handleFileConnection(fileConn);
                
                // Request files from the peer after both connections are established
                console.log('Requesting files from peer after connection:', peerId);
                requestPeerFiles(chatConn);
            });
        });
    };

    // Watch for folder changes and share files
    useEffect(() => {
        if (selectedFolder) {
            sharePeerFiles();
        }
    }, [selectedFolder]);

    // Handle chat connection
    const handleChatConnection = (conn) => {
        console.log('Setting up chat connection with:', conn.peer);
        
        chatConnectionsRef.current.set(conn.peer, conn);
        setPeers(prev => new Map(prev.set(conn.peer, conn)));

        // Request files from all peers, including the new one
        setTimeout(() => {
            requestFilesFromAllPeers();
            if (selectedFolder) {
                broadcastFileList();
            }
        }, 1000);

        conn.on('data', (data) => {
            console.log('Received chat data:', data.type, 'from:', conn.peer);
            
            switch (data.type) {
                case 'chat-message':
                    handleIncomingMessage(data, conn.peer);
                    break;
                case 'delete-message':
                    handleDeleteMessage(data);
                    break;
                case 'emoji':
                    handleIncomingEmoji(data, conn.peer);
                    break;
                case 'request-peer-files':
                    console.log('Received request for file list from:', conn.peer);
                    if (selectedFolder) {
                        console.log('Sharing files in response to request from:', conn.peer);
                        // Send files only to the requesting peer
                        sendFileListToPeer(conn);
                    }
                    break;
                case 'peer-file-list':
                    console.log('Received peer file list from:', conn.peer, 'Files:', data.files);
                    updatePeerFiles(conn.peer, data.files);
                    break;
                case 'file-request':
                    console.log('Received file request from:', conn.peer, 'File:', data.fileName);
                    handleFileRequest(data, conn);
                    break;
            }
        });

        conn.on('close', () => {
            console.log('Chat connection closed with:', conn.peer);
            chatConnectionsRef.current.delete(conn.peer);
            setPeers(prev => {
                const newPeers = new Map(prev);
                newPeers.delete(conn.peer);
                return newPeers;
            });
            setPeerFiles(prev => {
                const newMap = new Map(prev);
                newMap.delete(conn.peer);
                return newMap;
            });
        });
    };

    // Add constants for timing with much bigger delays and connection handling
    const PEER_FILE_CHECK_INTERVAL = 60000;    // 60 seconds
    const FILE_TRANSFER_COOLDOWN = 30000;      // 30 seconds
    const INITIAL_CHECK_DELAY = 10000;         // 10 seconds
    const CONNECTION_RETRY_DELAY = 5000;       // 5 seconds before retrying connection

    // Add connection check function
    const checkConnection = (conn) => {
        if (!conn || !conn.open || isReconnecting.current) {
            return false;
        }
        return true;
    };

    // Add message deduplication tracking
    const processedFileIds = useRef(new Set());

    // Update sendFile function to handle large files better
    const sendFile = async (file, fileId) => {
        if (!file || isFileTransferInProgress) {
            console.error('File transfer blocked - transfer in progress or no file');
            return;
        }

        // Check if already processed
        if (processedFileIds.current.has(fileId)) {
            console.log('File already being processed:', fileId);
            return;
        }
        processedFileIds.current.add(fileId);

        try {
            setIsFileTransferInProgress(true);
            console.log('Starting file transfer:', file.name, 'Size:', file.size);

            // Add file to messages with debounced updates
            setMessages(prev => {
                if (prev.some(msg => msg.id === fileId)) {
                    return prev;
                }
                return [...prev, {
                    id: fileId,
                    type: 'file',
                    fileName: file.name,
                    fileSize: formatFileSize(file.size),
                    sender: 'local',
                    timestamp: Date.now(),
                    status: 'Starting transfer...'
                }];
            });

            // Read file as ArrayBuffer
            const arrayBuffer = await file.arrayBuffer();
            const totalChunks = Math.ceil(arrayBuffer.byteLength / CHUNK_SIZE);
            const uint8Array = new Uint8Array(arrayBuffer);

            // Send to all connected peers
            const peers = Array.from(fileConnectionsRef.current.values());
            if (peers.length === 0) {
                throw new Error('No connected peers');
            }

            let lastProgressUpdate = 0;
            const PROGRESS_UPDATE_INTERVAL = 500; // Update UI every 500ms

            await Promise.all(peers.map(async (conn) => {
                if (!conn.open) return false;

                try {
                    // Send start message
                    conn.send({
                        type: FILE_TYPES.START,
                        fileId,
                        fileName: file.name,
                        fileSize: file.size,
                        totalChunks,
                        originalSender: peerInstance.current.id
                    });

                    // Send chunks with throttled UI updates
                    for (let i = 0; i < totalChunks; i++) {
                        const chunk = uint8Array.slice(i * CHUNK_SIZE, (i + 1) * CHUNK_SIZE);
                        conn.send({
                            type: FILE_TYPES.CHUNK,
                            fileId,
                            chunkIndex: i,
                            data: chunk.buffer
                        });
                        
                        // Throttle progress updates
                        const now = Date.now();
                        if (now - lastProgressUpdate >= PROGRESS_UPDATE_INTERVAL) {
                            const progress = Math.round((i + 1) * 100 / totalChunks);
                            setMessages(prev => prev.map(msg => 
                                msg.id === fileId ? {
                                    ...msg,
                                    status: `Sending... ${progress}%`
                                } : msg
                            ));
                            lastProgressUpdate = now;
                        }

                        // Add small delay between chunks to prevent UI blocking
                        if (i % 10 === 0) { // Every 10 chunks
                            await new Promise(resolve => setTimeout(resolve, 10));
                        }
                    }

                    // Send end message
                    conn.send({
                        type: FILE_TYPES.END,
                        fileId
                    });

                    return true;
                } catch (error) {
                    console.error(`Failed to send to peer ${conn.peer}:`, error);
                    return false;
                }
            }));

            setMessages(prev => prev.map(msg => 
                msg.id === fileId ? {
                    ...msg,
                    status: 'Transfer completed'
                } : msg
            ));

        } catch (error) {
            console.error('Error in file transfer:', error);
            setMessages(prev => prev.map(msg => 
                msg.id === fileId ? {
                    ...msg,
                    status: 'Failed to send'
                } : msg
            ));
        } finally {
            setIsFileTransferInProgress(false);
            setTimeout(() => {
                processedFileIds.current.delete(fileId);
            }, 5000);
        }
    };

    const handleFileReceive = useCallback((fileId, fileName, fileSize, sender, fileData) => {
        if (!fileData) {
            console.error('Invalid file data received');
            return;
        }

        // Ensure we have a valid file name
        const validFileName = fileName || 'received_file';

        // Check for duplicate
        if (processedFileIds.current.has(fileId)) {
            return;
        }
        processedFileIds.current.add(fileId);

        // Create a valid Blob from the file data if it's not already a Blob
        const fileBlob = fileData instanceof Blob ? fileData : new Blob([fileData]);

        setMessages(prev => {
            // Update existing or add new
            const existingIndex = prev.findIndex(msg => msg.id === fileId);
            if (existingIndex !== -1) {
                const updatedMessages = [...prev];
                updatedMessages[existingIndex] = {
                    ...updatedMessages[existingIndex],
                    status: 'Ready to download',
                    showDownload: true,
                    fileData: fileBlob
                };
                return updatedMessages;
            }

            return [...prev, {
                id: fileId,
                type: 'file',
                fileName: validFileName,
                fileSize: formatFileSize(fileSize || fileBlob.size),
                sender: 'remote',
                timestamp: Date.now(),
                status: 'Ready to download',
                showDownload: true,
                fileData: fileBlob,
                from: sender
            }];
        });

        // Remove from processed after a delay
        setTimeout(() => {
            processedFileIds.current.delete(fileId);
        }, 5000);
    }, []);

    // Update file transfer manager callbacks
    useEffect(() => {
        if (!fileTransferManagerRef.current) return;

        fileTransferManagerRef.current.setCallbacks({
            onProgress: (fileId, peerId, progress) => {
                if (!processedFileIds.current.has(fileId)) return;
                setMessages(prev => prev.map(msg => 
                    msg.id === fileId ? {
                        ...msg,
                        status: `${msg.sender === 'local' ? 'Sending' : 'Receiving'}... ${progress}%`
                    } : msg
                ));
            },
            onComplete: (fileId, file) => {
                if (file) {
                    handleFileReceive(fileId, file.name, file.size, 'remote', file);
                }
            },
            onError: (fileId, error) => {
                console.error('File transfer error:', error);
                processedFileIds.current.delete(fileId);
                setMessages(prev => prev.map(msg => 
                    msg.id === fileId ? {
                        ...msg,
                        status: 'Transfer failed'
                    } : msg
                ));
            }
        });
    }, [handleFileReceive]);

    // Update the requestPeerFiles function with connection checks
    const requestPeerFiles = (conn) => {
        if (!checkConnection(conn)) {
            console.log('Connection not ready for peer file request:', conn?.peer);
            return;
        }

        const now = Date.now();
        const lastRequestTime = lastPeerFileRequestTime.current[conn.peer] || 0;

        if (now - lastRequestTime < PEER_FILE_CHECK_INTERVAL) {
            const waitTime = Math.ceil((PEER_FILE_CHECK_INTERVAL - (now - lastRequestTime)) / 1000);
            console.log(`Skipping peer file request, need to wait ${waitTime}s more for:`, conn.peer);
            return;
        }

        console.log('Requesting file list from peer:', conn.peer);
        try {
            conn.send({
                type: 'request-peer-files'
            });
            lastPeerFileRequestTime.current[conn.peer] = now;
        } catch (error) {
            console.error('Error sending peer file request:', error);
            // Clear the connection if it's broken
            if (!conn.open) {
                fileConnectionsRef.current.delete(conn.peer);
            }
        }
    };

    // Update the handlePeerFileListRequest function
    const handlePeerFileListRequest = (conn) => {
        if (!checkConnection(conn)) {
            console.log('Connection not ready for file list request:', conn?.peer);
            return;
        }

        if (isFileTransferInProgress) {
            const delay = PEER_FILE_CHECK_INTERVAL * 2;
            console.log(`File transfer in progress, delaying peer file list request by ${delay/1000}s`);
            setTimeout(() => handlePeerFileListRequest(conn), delay);
            return;
        }

        const now = Date.now();
        const lastRequestTime = lastPeerFileRequestTime.current[conn.peer] || 0;

        if (now - lastRequestTime < PEER_FILE_CHECK_INTERVAL) {
            const delay = PEER_FILE_CHECK_INTERVAL - (now - lastRequestTime);
            console.log(`Delaying file list response to ${conn.peer} by ${Math.ceil(delay/1000)}s`);
            setTimeout(() => {
                if (checkConnection(conn)) {
                    sendFileList(conn);
                }
            }, delay);
            return;
        }

        console.log('Sending file list to peer:', conn.peer);
        sendFileList(conn);
        lastPeerFileRequestTime.current[conn.peer] = now;
    };

    // Update the checkPeerFiles function
    const checkPeerFiles = useCallback(() => {
        if (currentPage !== 'files' || isFileTransferInProgress) {
            return;
        }

        console.log('Checking peer files...');
        const peers = Array.from(fileConnectionsRef.current.values());
        if (peers.length === 0) return;

        peers.forEach(conn => {
            if (checkConnection(conn)) {
                requestPeerFiles(conn);
            }
        });
    }, [currentPage, isFileTransferInProgress]);

    // Handle file connection
    const handleFileConnection = useCallback((conn) => {
        console.log('Setting up file connection with peer:', conn.peer);
        setupFileConnection(conn);
        fileConnectionsRef.current.set(conn.peer, conn);
    }, []);

    // Share file list with peers
    const sharePeerFiles = async () => {
        if (!selectedFolder) {
            console.log('No folder selected, cannot share files');
            return;
        }
        
        try {
            const files = await getFilesFromFolder(selectedFolder);
            console.log('Sharing files with peers:', files);
            
            chatConnectionsRef.current.forEach((conn, peerId) => {
                if (conn.open) {
                    console.log('Sending file list to peer:', peerId);
                    conn.send({
                        type: 'peer-file-list',
                        files: files.map(file => ({
                            name: file.name,
                            size: file.size,
                            type: file.type,
                            lastModified: file.lastModified
                        }))
                    });
                } else {
                    console.log('Connection not open for peer:', peerId);
                }
            });
            setLocalFiles(files);
        } catch (error) {
            console.error('Error sharing file list:', error);
        }
    };

    // Handle file request
    const handleFileRequest = async (data, conn) => {
        console.log('Handling file request:', data);
        
        // Use selectedFolder for the peer who has the file (current peer)
        if (!selectedFolder) {
            console.log('No folder selected for responding peer');
            conn.send({
                type: 'file-error',
                fileId: data.fileId,
                error: 'Files not available'
            });
            return;
        }

        try {
            for await (const entry of selectedFolder.values()) {
                if (entry.kind === 'file' && entry.name === data.fileName) {
                    console.log('Found requested file:', data.fileName);
                    const file = await entry.getFile();
                    const arrayBuffer = await file.arrayBuffer();

                    // Get the file connection for this peer
                    const fileConn = fileConnectionsRef.current.get(data.requesterId);
                    if (!fileConn || !fileConn.open) {
                        console.error('No file connection available for peer:', data.requesterId);
                        conn.send({
                            type: 'file-error',
                            fileId: data.fileId,
                            error: 'File connection not available'
                        });
                        return;
                    }

                    // Send file through file connection
                    console.log('Sending file through file connection');
                    const totalChunks = Math.ceil(arrayBuffer.byteLength / CHUNK_SIZE);
                    const uint8Array = new Uint8Array(arrayBuffer);

                    // Send start message
                    fileConn.send({
                        type: FILE_TYPES.START,
                        fileId: data.fileId,
                        fileName: file.name,
                        fileSize: file.size,
                        totalChunks,
                        originalSender: peerInstance.current.id
                    });

                    // Send chunks
                    for (let i = 0; i < totalChunks; i++) {
                        const chunk = uint8Array.slice(i * CHUNK_SIZE, (i + 1) * CHUNK_SIZE);
                        fileConn.send({
                            type: FILE_TYPES.CHUNK,
                            fileId: data.fileId,
                            chunkIndex: i,
                            data: chunk.buffer
                        });

                        // Add small delay every few chunks to prevent overwhelming
                        if (i % 10 === 0) {
                            await new Promise(resolve => setTimeout(resolve, 10));
                        }
                    }

                    // Send end message
                    fileConn.send({
                        type: FILE_TYPES.END,
                        fileId: data.fileId
                    });

                    console.log('File sent:', file.name);
                    return;
                }
            }
            
            // File not found
            conn.send({
                type: 'file-error',
                fileId: data.fileId,
                error: 'File not found'
            });
        } catch (error) {
            console.error('Error sending file:', error);
            conn.send({
                type: 'file-error',
                fileId: data.fileId,
                error: 'Failed to send file'
            });
        }
    };

    // Update peer files list
    const updatePeerFiles = (peerId, files) => {
        setPeerFiles(prev => {
            const newMap = new Map(prev);
            newMap.set(peerId, files);
            return newMap;
        });
    };

    // Handle file transfer
    const handleFileTransfer = (data) => {
        try {
            console.log('Received file transfer:', data.fileName);
            
            // Create blob and trigger download
            const blob = new Blob([data.fileData]);
            const url = URL.createObjectURL(blob);

            // Create download link and trigger download
            const a = document.createElement('a');
            a.href = url;
            a.download = data.fileName;
            document.body.appendChild(a);
            a.click();
            document.body.removeChild(a);
            URL.revokeObjectURL(url);

            // Update message status
            setMessages(prev => prev.map(msg => 
                msg.id === data.fileId ? {
                    ...msg,
                    fileName: data.fileName,
                    fileSize: formatFileSize(data.fileSize),
                    status: 'Downloaded'
                } : msg
            ));

            // Clean up URL after a delay
            setTimeout(() => URL.revokeObjectURL(url), 1000);
        } catch (error) {
            console.error('Error handling file transfer:', error);
            handleFileError({
                fileId: data.fileId,
                error: 'Failed to download file'
            });
        }
    };

    // Handle incoming messages
    const handleIncomingMessage = (data, senderId) => {
        if (!data.messageId || sentMessages.has(data.messageId)) return;
        
        sentMessages.add(data.messageId);
        
        setMessages(prev => [...prev, {
            id: data.messageId,
            text: data.text,
            type: data.type || 'text',
            sender: data.originalSender === senderId ? 'remote' : 'forwarded',
            senderName: `Peer ${senderId.slice(0, 5)}`,
            timestamp: data.timestamp,
            fileData: data.fileData
        }]);

        // Forward to other peers if not already forwarded
        if (!data.forwarded) {
            forwardToOtherPeers({ ...data, forwarded: true }, senderId);
        }
    };

    // Add file receiving logic
    const handleIncomingFile = (data, senderId) => {
        if (!data.messageId || sentMessages.has(data.messageId)) return;
        
        try {
            sentMessages.add(data.messageId);
            console.log('Processing incoming file:', data.fileName);
            
            const fileUrl = URL.createObjectURL(data.fileData);
            setMessages(prev => [...prev, {
                id: data.messageId,
                type: 'file',
                fileName: data.fileName,
                fileSize: formatFileSize(data.fileSize),
                fileData: fileUrl,
                sender: data.originalSender === senderId ? 'remote' : 'forwarded',
                senderName: `Peer ${senderId.slice(0, 5)}`,
                timestamp: data.timestamp
            }]);

            // Save file to selected folder if available
            if (selectedFolder) {
                saveReceivedFile(data.fileName, data.fileData);
            }

            // Forward to other peers
            forwardToOtherPeers(data, senderId);
        } catch (error) {
            console.error('Error handling incoming file:', error);
        }
    };

    const handleIncomingEmoji = (data, senderId) => {
        if (!data.messageId || sentMessages.has(data.messageId)) return;
        
        sentMessages.add(data.messageId);
        
        setMessages(prev => [...prev, {
            id: data.messageId,
            type: 'emoji',
            text: data.emoji,
            sender: data.originalSender === senderId ? 'remote' : 'forwarded',
            senderName: `Peer ${senderId.slice(0, 5)}`,
            timestamp: data.timestamp
        }]);

        // Forward to other peers if not already forwarded
        if (!data.forwarded) {
            forwardToOtherPeers({ ...data, forwarded: true }, senderId);
        }
    };

    const forwardToOtherPeers = (data, senderId) => {
        chatConnectionsRef.current.forEach((conn, peerId) => {
            if (peerId !== senderId && conn.open) {
                console.log(`Forwarding file to peer: ${peerId}`);
                conn.send(data);
            }
        });
    };

    // Send message
    const sendMessage = () => {
        if (!message.trim()) return;

        const messageData = {
            type: 'chat-message',
            messageId: Date.now().toString(36) + Math.random().toString(36).substr(2),
            text: message.trim(),
            timestamp: Date.now(),
            originalSender: peerInstance.current.id
        };

        setMessages(prev => [...prev, {
            id: messageData.messageId,
            text: messageData.text,
            sender: 'local',
            senderName: 'You',
            timestamp: messageData.timestamp
        }]);

        // Send only through chat connections
        chatConnectionsRef.current.forEach(conn => {
            if (conn.open) {
                conn.send(messageData);
            }
        });

        setMessage('');
    };

    // Add this helper function for file size formatting
    const formatFileSize = (bytes) => {
        if (bytes === 0) return '0 Bytes';
        const k = 1024;
        const sizes = ['Bytes', 'KB', 'MB', 'GB'];
        const i = Math.floor(Math.log(bytes) / Math.log(k));
        return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
    };

    const sendEmoji = (emoji) => {
        if (!emoji || peers.size === 0) return;

        const messageData = {
            type: 'emoji',
            messageId: Date.now().toString(36) + Math.random().toString(36).substr(2),
            emoji: emoji,
            timestamp: Date.now(),
            originalSender: peerInstance.current.id,
            forwarded: false
        };

        setMessages(prev => [...prev, {
            id: messageData.messageId,
            type: 'emoji',
            text: emoji,
            sender: 'local',
            senderName: 'You',
            timestamp: messageData.timestamp
        }]);

        chatConnectionsRef.current.forEach((conn, peerId) => {
            if (conn.open) {
                try {
                    conn.send(messageData);
                } catch (error) {
                    console.error('Error sending emoji to peer:', peerId, error);
                }
            }
        });
    };

    const broadcastToPeers = (data) => {
        peers.forEach(conn => {
            if (conn.open) {
                conn.send(data);
            }
        });
    };

    // Add a new function to request files from all peers
    const requestFilesFromAllPeers = () => {
        console.log('Requesting files from all peers');
        chatConnectionsRef.current.forEach((conn, peerId) => {
            if (conn.open) {
                console.log('Requesting files from peer:', peerId);
                requestPeerFiles(conn);
            }
        });
    };

    // Add a new function to broadcast file list to all peers
    const broadcastFileList = async () => {
        if (!selectedFolder) {
            console.log('No folder selected, cannot broadcast files');
            return;
        }
        
        try {
            const files = await getFilesFromFolder(selectedFolder);
            console.log('Broadcasting file list to all peers:', files);
            
            chatConnectionsRef.current.forEach((conn, peerId) => {
                if (conn.open) {
                    console.log('Broadcasting file list to peer:', peerId);
                    conn.send({
                        type: 'peer-file-list',
                        files: files.map(file => ({
                            name: file.name,
                            size: file.size,
                            type: file.type,
                            lastModified: file.lastModified
                        }))
                    });
                }
            });
            setLocalFiles(files);
        } catch (error) {
            console.error('Error broadcasting file list:', error);
        }
    };

    // Add function to send file list to a specific peer
    const sendFileListToPeer = async (conn) => {
        if (!selectedFolder) {
            console.log('No folder selected, cannot send file list');
            return;
        }

        try {
            const files = await getFilesFromFolder(selectedFolder);
            console.log('Sending file list to peer:', conn.peer, 'Files:', files);
            
            if (conn.open) {
                conn.send({
                    type: 'peer-file-list',
                    files: files.map(file => ({
                        name: file.name,
                        size: file.size,
                        type: file.type,
                        lastModified: file.lastModified
                    }))
                });
            }
        } catch (error) {
            console.error('Error sending file list to peer:', error);
        }
    };

    // Update handleFolderSelect function
    const handleFolderSelect = async (folderHandle) => {
        if (folderHandle) {
            console.log('Folder selected:', folderHandle);
            setSelectedFolder(folderHandle);
            setShowFolderPopup(false);
            
            try {
                const files = await getFilesFromFolder(folderHandle);
                console.log('Files in selected folder:', files);
                setLocalFiles(files);
                
                // Broadcast our files to all peers
                setTimeout(() => {
                    broadcastFileList();
                    requestFilesFromAllPeers();
                }, 1000);
            } catch (error) {
                console.error('Error processing folder:', error);
            }
        }
    };

    // Update the periodic file list sync interval
    useEffect(() => {
        let syncInterval;
        
        // Only start the interval if we're in the files tab and have a selected folder
        if (currentPage === 'files' && selectedFolder && !isFileTransferInProgress) {
            console.log('Starting periodic file sync - files tab active');
            syncInterval = setInterval(() => {
                console.log('Performing periodic file list sync');
                requestFilesFromAllPeers();
                broadcastFileList();
            }, 30000);
        }

        return () => {
            if (syncInterval) {
                console.log('Stopping periodic file sync - leaving files tab');
                clearInterval(syncInterval);
            }
        };
    }, [currentPage, selectedFolder, isFileTransferInProgress]);

    // Add missing functions
    const handleDeleteMessage = (data, senderId) => {
        console.log('Handling delete message:', data.messageId, 'from:', senderId);
        
        // Remove the message locally
        setMessages(prev => {
            const newMessages = prev.filter(msg => msg.id !== data.messageId);
            console.log('Filtered messages:', prev.length, '->', newMessages.length);
            return newMessages;
        });

        // Forward to other peers if not already forwarded
        if (!data.forwarded) {
            const forwardData = {
                ...data,
                forwarded: true
            };
            console.log('Forwarding delete message to other peers');
            chatConnectionsRef.current.forEach((conn, peerId) => {
                if (peerId !== senderId && conn.open) {
                    try {
                        conn.send(forwardData);
                        console.log('Forwarded delete to peer:', peerId);
                    } catch (error) {
                        console.error('Error forwarding delete to peer:', peerId, error);
                    }
                }
            });
        }
    };

    const requestFile = async (fileName, peerId) => {
        const chatConn = chatConnectionsRef.current.get(peerId);
        const fileConn = fileConnectionsRef.current.get(peerId);
        
        if (!chatConn?.open || !fileConn?.open) {
            console.error('No connections available for peer:', peerId);
            return;
        }

        const fileId = `file-${Date.now()}-${Math.random().toString(36).substr(2)}`;

        // Add a pending message to show status
        setMessages(prev => [...prev, {
            id: fileId,
            type: 'file',
            fileName: fileName,
            sender: 'local',
            timestamp: Date.now(),
            status: 'Requesting file...'
        }]);

        // Send file request through chat connection
        chatConn.send({
            type: 'file-request',
            fileName,
            fileId,
            requesterId: peerInstance.current.id
        });
    };

    // Add missing functions
    const saveReceivedFile = async (fileName, fileData) => {
        if (!selectedFolder) {
            console.error('No folder selected to save file');
            return;
        }

        try {
            console.log('Saving received file:', fileName);
            const fileHandle = await selectedFolder.getFileHandle(fileName, { create: true });
            const writable = await fileHandle.createWritable();
            await writable.write(fileData);
            await writable.close();
            console.log('File saved successfully:', fileName);
        } catch (error) {
            console.error('Error saving file:', error);
        }
    };

    const handleFileSearch = async (query) => {
        setIsSearching(true);
        const results = [];
        
        try {
            // Search local files
            const localMatches = localFiles.filter(file => 
                file.name.toLowerCase().includes(query.toLowerCase())
            );
            results.push(...localMatches.map(file => ({
                ...file,
                isLocal: true
            })));

            // Search peer files
            peerFiles.forEach((files, peerId) => {
                const matches = files.filter(file =>
                    file.name.toLowerCase().includes(query.toLowerCase())
                );
                results.push(...matches.map(file => ({
                    ...file,
                    peerId,
                    isLocal: false
                })));
            });

            setSearchResults(results);
        } catch (error) {
            console.error('Error searching files:', error);
        } finally {
            setIsSearching(false);
        }
    };

    // Add HomePage component
    const HomePage = () => (
        <HomeContainer>
            <HeroSection>
                <HeroContent>
                    <h1>Connect and Share Instantly</h1>
                    <p>Secure P2P communication with end-to-end encryption</p>
                    <StartButton onClick={() => setCurrentPage('groupchat')}>
                        Start Chatting
                    </StartButton>
                </HeroContent>
            </HeroSection>

            <FeaturesSection>
                <FeatureCard>
                    <FaUsers size={40} />
                    <h3>Group Chat</h3>
                    <p>Connect with multiple peers simultaneously</p>
                </FeatureCard>
                <FeatureCard>
                    <span role="img" aria-label="security" style={{fontSize: '40px'}}>🔒</span>
                    <h3>Secure</h3>
                    <p>Direct peer-to-peer connections</p>
                </FeatureCard>
                <FeatureCard>
                    <span role="img" aria-label="speed" style={{fontSize: '40px'}}>⚡</span>
                    <h3>Fast</h3>
                    <p>Real-time messaging and file sharing</p>
                </FeatureCard>
            </FeaturesSection>
        </HomeContainer>
    );

    // Add ChatInterface component
    const ChatInterface = () => (
        <ChatContainer>
            <GroupInfo>
                <GroupName>Group Chat</GroupName>
                <PeerCount>
                    <OnlineIndicator active={peers.size > 0} />
                    {peers.size} peers connected
                </PeerCount>
            </GroupInfo>
            
            <StatusBar>{status}</StatusBar>

            <ChatSection>
                <MessagesContainer>
                    {messages.map((msg, index) => (
                        <Message key={index} sender={msg.sender}>
                            <MessageHeader>
                                <SenderName>{msg.senderName}</SenderName>
                                {msg.sender === 'local' && (
                                    <DeleteButton onClick={() => deleteMessage(msg.id, msg.sender)}>
                                        🗑️
                                    </DeleteButton>
                                )}
                            </MessageHeader>
                            {msg.type === 'file' ? (
                                <FileMessage>
                                    <FileIcon>📎</FileIcon>
                                    <FileDetails>
                                        <div>{msg.fileName}</div>
                                        <div>{msg.fileSize}</div>
                                        {msg.status && <div>{msg.status}</div>}
                                    </FileDetails>
                                    {msg.showDownload && (
                                        <DownloadButton onClick={() => handleDownload(msg)}>
                                            Download
                                        </DownloadButton>
                                    )}
                                </FileMessage>
                            ) : (
                                <MessageText sender={msg.sender}>{msg.text}</MessageText>
                            )}
                        </Message>
                    ))}
                    <div ref={messagesEndRef} />
                </MessagesContainer>

                <InputSection>
                    <MessageInputWrapper>
                        <QuickEmojis>
                            {['👋', '❤️', '👍'].map(emoji => (
                                <EmojiButton 
                                    key={emoji} 
                                    onClick={() => sendEmoji(emoji)}
                                >
                                    {emoji}
                                </EmojiButton>
                            ))}
                        </QuickEmojis>
                        <MessageInput
                            value={message}
                            onChange={(e) => setMessage(e.target.value)}
                            placeholder="Type a message..."
                            onKeyPress={(e) => e.key === 'Enter' && sendMessage()}
                            autoFocus
                        />
                        <ActionButtons>
                            <FileInput
                                id="file-input"
                                type="file"
                                onChange={handleFileSelect}
                                onClick={(e) => e.target.value = null}
                            />
                            <ActionButton as="label" htmlFor="file-input" title="Share File">
                                +
                            </ActionButton>
                            <ActionButton onClick={sendMessage}>
                                ➤
                            </ActionButton>
                        </ActionButtons>
                    </MessageInputWrapper>
                </InputSection>
            </ChatSection>
        </ChatContainer>
    );

    // Add requestFileFromPeer function
    const requestFileFromPeer = (fileName, peerId) => {
        console.log('Requesting file:', fileName, 'from peer:', peerId);
        const conn = chatConnectionsRef.current.get(peerId);
        if (conn && conn.open) {
            conn.send({
                type: 'file-request',
                fileName,
                requesterId: peerInstance.current.id
            });
            console.log('File request sent to peer:', peerId);
        } else {
            console.error('No chat connection available for peer:', peerId);
        }
    };

    // Add renderPage function
    const renderPage = () => {
        switch(currentPage) {
            case 'home':
                return <HomePage />;
            case 'groupchat':
                return <ChatInterface />;
            case 'files':
                return (
                    <PeerFilesContainer>
                        <h2>Available Files from Peers</h2>
                        {Array.from(peerFiles.entries()).map(([peerId, files]) => (
                            <PeerSection key={peerId}>
                                <PeerHeader>
                                    <h3>Peer: {peerId.slice(0, 8)}...</h3>
                                </PeerHeader>
                                <FileList>
                                    {files.map((file, index) => (
                                        <FileItem key={index}>
                                            <FileInfo>
                                                <FileName>{file.name}</FileName>
                                                <FileSize>{formatFileSize(file.size)}</FileSize>
                                            </FileInfo>
                                            <RequestFileButton 
                                                onClick={() => requestFileFromPeer(file.name, peerId)}
                                            >
                                                Request File
                                            </RequestFileButton>
                                        </FileItem>
                                    ))}
                                </FileList>
                            </PeerSection>
                        ))}
                        {peerFiles.size === 0 && (
                            <EmptyState>
                                No peer files available. Wait for peers to connect and share their files.
                            </EmptyState>
                        )}
                    </PeerFilesContainer>
                );
            case 'manage-folder':
                return <ManageFolder selectedFolder={selectedFolder} />;
            default:
                return <HomePage />;
        }
    };

    // Add deleteMessage function
    const deleteMessage = useCallback((messageId, sender) => {
        if (sender !== 'local') return;

        console.log('Initiating message deletion:', messageId);

        // Create deletion message
        const deleteData = {
            type: 'delete-message',
            messageId: messageId,
            timestamp: Date.now(),
            originalSender: peerInstance.current.id,
            forwarded: false
        };

        // Delete locally first
        handleDeleteMessage(deleteData);

        // Send to all peers
        console.log('Broadcasting delete message to all peers');
        chatConnectionsRef.current.forEach((conn, peerId) => {
            if (conn.open) {
                try {
                    console.log('Sending delete message to peer:', peerId);
                    conn.send(deleteData);
                } catch (error) {
                    console.error('Error sending delete to peer:', peerId, error);
                }
            }
        });
    }, []);

    // Add downloadFile function
    const downloadFile = (fileUrl, fileName) => {
        try {
            const a = document.createElement('a');
            a.href = fileUrl;
            a.download = fileName;
            document.body.appendChild(a);
            a.click();
            document.body.removeChild(a);
            console.log('File download initiated:', fileName);
        } catch (error) {
            console.error('Error downloading file:', error);
        }
    };

    // Update handleFileInputChange function
    const handleFileSelect = async (event) => {
        const file = event.target.files[0];
        if (!file) return;

        const fileId = `file-${Date.now()}`;

        // Add single message to chat for the sending peer
        setMessages(prev => [...prev, {
            id: fileId,
            type: 'file',
            fileName: file.name,
            fileSize: formatFileSize(file.size),
            sender: 'local',
            timestamp: Date.now(),
            status: 'Starting transfer...'
        }]);

        // Send file through group chat
        await sendFile(file, fileId);

        // Clear the input
        event.target.value = '';
    };

    // Update the chat data handler
    const handleChatData = useCallback((data, conn) => {
        if (!data || !data.type) return;
        
        console.log('Received chat data:', data.type, 'from:', conn.peer);

        switch (data.type) {
            case 'chat-message':
                handleIncomingMessage(data, conn.peer);
                break;
            case 'file-request':
                handleFileRequest(data, conn);
                break;
            case 'file-transfer':
                handleFileTransfer(data);
                break;
            case 'file-error':
                handleFileError(data);
                break;
            case 'delete-message':
                handleDeleteMessage(data);
                if (!data.forwarded) {
                    forwardToOtherPeers({ ...data, forwarded: true }, conn.peer);
                }
                break;
            case 'request-peer-files':
                if (checkConnection(conn)) {
                    handlePeerFileListRequest(conn);
                }
                break;
            case 'peer-file-list':
                if (!isFileTransferInProgress) {
                    updatePeerFiles(conn.peer, data.files);
                }
                break;
        }
    }, [handleIncomingMessage, handlePeerFileListRequest, updatePeerFiles, isFileTransferInProgress]);

    // Update peer file checking initialization
    useEffect(() => {
        // Clear any existing interval
        if (peerFileCheckIntervalRef.current) {
            clearInterval(peerFileCheckIntervalRef.current);
            peerFileCheckIntervalRef.current = null;
        }

        // Only start checking if in files tab
        if (activeTab === 'files') {
            console.log('Starting file checking - in files tab');
            peerFileCheckIntervalRef.current = setInterval(checkPeerFiles, PEER_FILE_CHECK_INTERVAL);
            // Initial check
            checkPeerFiles();
        }

        return () => {
            if (peerFileCheckIntervalRef.current) {
                clearInterval(peerFileCheckIntervalRef.current);
                peerFileCheckIntervalRef.current = null;
            }
        };
    }, [activeTab]);

    // Update navigation component
    const handleTabChange = (tab) => {
        setActiveTab(tab);
        isFileTabActive.current = tab === 'files';
        
        // Clear any existing interval when changing tabs
        if (peerFileCheckIntervalRef.current) {
            clearInterval(peerFileCheckIntervalRef.current);
            peerFileCheckIntervalRef.current = null;
            console.log('Stopped peer file checking - tab changed');
        }
    };

    // Add setupFileConnection function
    const setupFileConnection = useCallback((conn) => {
        let fileBuffer = null;
        let fileName = null;
        let fileSize = 0;
        let receivedChunks = new Map();
        let totalChunks = 0;

        conn.on('data', async (data) => {
            if (!data.type || !data.fileId) return;

            switch (data.type) {
                case FILE_TYPES.START:
                    console.log('Starting file reception:', data.fileName);
                    fileName = data.fileName;
                    fileSize = data.fileSize;
                    totalChunks = data.totalChunks;
                    receivedChunks = new Map();
                    break;

                case FILE_TYPES.CHUNK:
                    if (!fileName) return;
                    receivedChunks.set(data.chunkIndex, new Uint8Array(data.data));
                    
                    // Update progress
                    const progress = Math.round(receivedChunks.size * 100 / totalChunks);
                    setMessages(prev => prev.map(msg => 
                        msg.id === data.fileId ? {
                            ...msg,
                            status: `Receiving... ${progress}%`
                        } : msg
                    ));
                    break;

                case FILE_TYPES.END:
                    if (!fileName || receivedChunks.size !== totalChunks) return;

                    // Combine chunks
                    const chunks = Array.from(receivedChunks.values());
                    const totalLength = chunks.reduce((sum, chunk) => sum + chunk.length, 0);
                    const combinedArray = new Uint8Array(totalLength);
                    let offset = 0;
                    chunks.forEach(chunk => {
                        combinedArray.set(chunk, offset);
                        offset += chunk.length;
                    });

                    // Create file blob
                    const fileBlob = new Blob([combinedArray]);
                    handleFileReceive(data.fileId, fileName, fileSize, conn.peer, fileBlob);

                    // Reset state
                    fileName = null;
                    fileSize = 0;
                    receivedChunks = new Map();
                    totalChunks = 0;
                    break;
            }
        });

        conn.on('close', () => {
            console.log('File connection closed with peer:', conn.peer);
            fileConnectionsRef.current.delete(conn.peer);
        });
    }, [handleFileReceive]);

    // Add sendFileList function
    const sendFileList = async (conn) => {
        if (!selectedFolder) {
            console.log('No folder selected, cannot send file list');
            return;
        }

        try {
            const files = await getFilesFromFolder(selectedFolder);
            console.log('Sending file list to peer:', conn.peer, 'Files:', files);
            
            if (conn.open) {
                conn.send({
                    type: 'peer-file-list',
                    files: files.map(file => ({
                        name: file.name,
                        size: file.size,
                        type: file.type,
                        lastModified: file.lastModified
                    }))
                });
            }
        } catch (error) {
            console.error('Error sending file list to peer:', error);
        }
    };

    // Add handleDownload function
    const handleDownload = useCallback((message) => {
        if (!message.fileData) {
            console.error('No file data available for download');
            return;
        }

        try {
            const blob = new Blob([message.fileData]);
            const url = URL.createObjectURL(blob);
            const a = document.createElement('a');
            a.href = url;
            a.download = message.fileName;
            document.body.appendChild(a);
            a.click();
            document.body.removeChild(a);

            setMessages(prev => prev.map(msg => 
                msg.id === message.id ? {
                    ...msg,
                    status: 'Downloaded'
                } : msg
            ));

            // Clean up URL after a delay
            setTimeout(() => URL.revokeObjectURL(url), 1000);
        } catch (error) {
            console.error('Download failed:', error);
            setMessages(prev => prev.map(msg => 
                msg.id === message.id ? {
                    ...msg,
                    status: 'Download failed'
                } : msg
            ));
        }
    }, []);

    // Add deletion confirmation handler
    const handleDeletionConfirmation = (data, conn) => {
        if (!data.messageId) return;
        
        // Send confirmation back
        if (conn.open) {
            conn.send({
                type: 'deletion-confirmed',
                messageId: data.messageId
            });
        }

        // Delete the message if we still have it
        handleDeleteMessage(data);
    };

    // Add handleFileResponse function
    const handleFileResponse = (data) => {
        try {
            // Create blob and download URL
            const blob = new Blob([data.fileData]);
            const url = URL.createObjectURL(blob);

            // Create download link and trigger download
            const a = document.createElement('a');
            a.href = url;
            a.download = data.fileName;
            document.body.appendChild(a);
            a.click();
            document.body.removeChild(a);

            // Update message status
            setMessages(prev => prev.map(msg => 
                msg.id === data.fileId ? {
                    ...msg,
                    fileName: data.fileName,
                    fileSize: formatFileSize(data.fileSize),
                    status: 'Downloaded'
                } : msg
            ));

            // Clean up URL after a delay
            setTimeout(() => URL.revokeObjectURL(url), 1000);
        } catch (error) {
            console.error('Error handling file response:', error);
            handleFileError({
                fileId: data.fileId,
                error: 'Failed to download file'
            });
        }
    };

    // Add handleFileError function
    const handleFileError = (data) => {
        console.log('File error:', data.error);
        setMessages(prev => prev.map(msg => 
            msg.id === data.fileId ? {
                ...msg,
                status: data.error || 'Error occurred'
            } : msg
        ));
    };

    // Main app return with navigation
    return (
        <AppContainer>
            {showFolderPopup && <FolderSelectionPopup onSelect={handleFolderSelect} />}
            <Navbar>
                <NavLogo onClick={() => setCurrentPage('home')}>
                    <FaHome size={24} />
                    P2P Share
                </NavLogo>
                <NavLinks>
                    <NavItem 
                        active={currentPage === 'home'} 
                        onClick={() => setCurrentPage('home')}
                    >
                        <FaHome /> Home
                    </NavItem>
                    <NavItem 
                        active={currentPage === 'groupchat'} 
                        onClick={() => setCurrentPage('groupchat')}
                    >
                        <FaUsers /> Group Chat
                    </NavItem>
                    <NavItem 
                        active={currentPage === 'files'} 
                        onClick={() => setCurrentPage('files')}
                    >
                        <FaFolder /> Files
                    </NavItem>
                    <NavItem 
                        active={currentPage === 'manage-folder'} 
                        onClick={() => setCurrentPage('manage-folder')}
                    >
                        <FaCog /> Manage Folder
                    </NavItem>
                </NavLinks>
            </Navbar>
            
            {showSearch && (
                <SearchBar>
                    <SearchInput
                        type="text"
                        placeholder="Search files..."
                        value={searchQuery}
                        onChange={(e) => {
                            setSearchQuery(e.target.value);
                            handleFileSearch(e.target.value);
                        }}
                    />
                    {isSearching && <SearchingIndicator>Searching...</SearchingIndicator>}
                    <SearchResults>
                        {searchResults.length > 0 ? (
                            searchResults.map((file, index) => (
                                <SearchResult key={index}>
                                    <FileInfo>
                                        <FileName>{file.name}</FileName>
                                        <FileSize>{(file.size / 1024).toFixed(2)} KB</FileSize>
                                    </FileInfo>
                                    <PeerInfo>
                                        {file.isLocal ? 'Local File' : `From: Peer ${file.peerId.slice(0, 5)}`}
                                    </PeerInfo>
                                    {!file.isLocal && (
                                        <RequestButton onClick={() => requestFile(file.name, file.peerId)}>
                                            Request
                                        </RequestButton>
                                    )}
                                </SearchResult>
                            ))
                        ) : searchQuery && !isSearching ? (
                            <NoResults>No files found</NoResults>
                        ) : null}
                    </SearchResults>
                </SearchBar>
            )}

            <MainContent>
                {renderPage()}
            </MainContent>
        </AppContainer>
    );
} // Add closing brace for App component

export default App;
