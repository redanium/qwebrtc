#pragma once

#include <QObject>
#include "qwebrtcsessiondescription.hpp"

class QWebRTCPeerConnection_impl;

class QWebRTCDataChannel;
class QWebRTCIceCandidate;
class QWebRTCSessionDescription;

class QWebRTCMediaStream;

class QWebRTCPeerConnection : public QObject {
    Q_OBJECT
    friend class QWebRTCPeerConnectionFactory;
public:
    enum class SignalingState {
        Stable,
        LocalOffer,
        LocalOfferPrAnswer,
        RemoteOffer,
        RemotePrAnswer,
        Closed
    };

    void createOfferForConstraints(const QVariantMap& constraints,
            std::function<void(const std::shared_ptr<QWebRTCSessionDescription>&)>);
    void createAnswerForConstraints(const QVariantMap& constraints,
            std::function<void(const std::shared_ptr<QWebRTCSessionDescription>&)>);

    void addStream(const std::shared_ptr<QWebRTCMediaStream>&);
    void removeStream(const std::shared_ptr<QWebRTCMediaStream>&);

    void setConfiguration();

    void setLocalDescription(const std::shared_ptr<QWebRTCSessionDescription>&, std::function<void(bool)>);
    void setRemoteDescription(const std::shared_ptr<QWebRTCSessionDescription>&, std::function<void(bool)>);

    void addIceCandidate(const std::shared_ptr<QWebRTCIceCandidate>&);
    void removeIceCandidate(const std::shared_ptr<QWebRTCIceCandidate>&);

    std::shared_ptr<QWebRTCDataChannel> dataChannelForLabel(const QString& label);

    static std::shared_ptr<QWebRTCSessionDescription> createSessionDescription(QWebRTCSessionDescription::SDPType, const QByteArray&);
    static std::shared_ptr<QWebRTCIceCandidate> createIceCandidate(QByteArray mId, int sdpMLineIndex, const QByteArray& sdpData);

    std::shared_ptr<QWebRTCSessionDescription> currentLocalDescription();
    std::shared_ptr<QWebRTCSessionDescription> currentRemoteDescription();

    SignalingState signalingState();

Q_SIGNALS:
    void signalingChange();
    void streamAdded(const std::shared_ptr<QWebRTCMediaStream>&);
    void streamRemoved(const std::shared_ptr<QWebRTCMediaStream>&);
    void newIceCandidate(const std::shared_ptr<QWebRTCIceCandidate>&);
    void iceCandidateRemoved(std::shared_ptr<QWebRTCIceCandidate>);
    void renegotiationNeeded();

private:
    QWebRTCPeerConnection();
    std::shared_ptr<QWebRTCPeerConnection_impl> m_impl;
};
