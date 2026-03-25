import React, { createContext, useState, useContext } from 'react';

const PeerFileContext = createContext();

export const usePeerFiles = () => {
    return useContext(PeerFileContext);
};

export const PeerFileProvider = ({ children }) => {
    const [peerFiles, setPeerFiles] = useState(new Map());

    const updatePeerFiles = (peerId, files) => {
        setPeerFiles(prev => {
            const newMap = new Map(prev);
            newMap.set(peerId, files);
            return newMap;
        });
    };

    return (
        <PeerFileContext.Provider value={{ peerFiles, updatePeerFiles }}>
            {children}
        </PeerFileContext.Provider>
    );
};