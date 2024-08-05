#pragma once

#include <QThread>

class CodecThread : public QThread
{
    Q_OBJECT

public:
    enum class EMode
    {
        Encode,
        Decode
    };

    CodecThread(QObject* parent, const QString& source_filepath, const QString& target_filepath, const EMode mode);

    virtual void run() override;

signals:
    void error(const QString& message);

private:
    const QString m_source_filepath;
    const QString m_target_filepath;
    const EMode m_mode;
};

