#ifndef FILEJOB_H
#define FILEJOB_H

#include <QObject>
#include <QMap>
#include <QElapsedTimer>
#include <QUrl>
#include "durl.h"
#include <QStorageInfo>

#define TRANSFER_RATE 5
#define MSEC_FOR_DISPLAY 1000
#define DATA_BLOCK_SIZE 65536
#define ONE_MB_SIZE 1048576
#define ONE_KB_SIZE 1024


class FileJob : public QObject
{
    Q_OBJECT
public:
    enum Status
    {
        Started,
        Paused,
        Cancelled,
        Run,
        Conflicted
    };

    enum JobType{
        Copy,
        Move,
        Delete,
        Restore
    };
    Q_ENUM(JobType)

    static int FileJobCount;

    static qint64 Msec_For_Display;
    static qint64 Data_Block_Size;
    static qint64 Data_Flush_Size;


    explicit FileJob(JobType jobType, QObject *parent = 0);
    ~FileJob();
    void setStatus(Status status);
    void setJobId(const QString &id);
    QString getJobId();
    QString checkDuplicateName(const QString &name);
    void setApplyToAll(bool v);
    void setReplace(bool v);

    int getWindowId();

    QString getTargetDir();

    inline QMap<QString, QString> jobDetail(){ return m_jobDetail; }
    inline qint64 currentMsec() { return m_timer.elapsed(); }
    inline qint64 lastMsec() { return m_lastMsec; }
    inline bool isJobAdded() { return m_isJobAdded; }

signals:

    /*add copy/move/delete job to taskdialog when copy/move/delete job created*/
    void requestJobAdded(const QMap<QString, QString>& jobDetail);

    /*remove copy/move/delete job to taskdialog when copy/move/delete job finished*/
    void requestJobRemoved(const QMap<QString, QString>& jobDetail);

    /*update copy/move/delete job taskdialog ui*/
    void requestJobDataUpdated(const QMap<QString, QString>& jobDetail,
                           const QMap<QString, QString>& data);

    /*abort copy/move/delete job taskdialog from ui*/
    void requestAbortTask(const QMap<QString, QString>& jobDetail);

    /*copy/move job conflict dialog show */
    void requestConflictDialogShowed(const QMap<QString, QString>& jobDetail);

    void progressPercent(int value);
    void error(QString content);
    void result(QString content);
    void finished();

public slots:
    DUrlList doCopy(const DUrlList &files, const QString &destination);
    void doDelete(const DUrlList &files);
    DUrlList doMoveToTrash(const DUrlList &files);
    DUrlList doMove(const DUrlList &files, const QString &destination);

    void doTrashRestore(const QString &srcFile, const QString& tarFile);

    void paused();
    void started();
    void cancelled();
    void handleJobFinished();

    void jobUpdated();
    void jobAdded();
    void jobRemoved();
    void jobAborted();
    void jobPrepared();
    void jobConflicted();

private:
    Status m_status;
    QString m_trashLoc;
    QString m_id;
    QMap<QString, QString> m_jobDetail;
    qint64 m_bytesCopied;
    qint64 m_totalSize;
    qint64 m_bytesPerSec;
    QString m_progress;
    float m_factor;
    bool m_isJobAdded = false;
    QString m_srcFileName;
    QString m_tarFileName;
    QString m_srcPath;
    QString m_tarPath;
    QElapsedTimer m_timer;
    qint64 m_lastMsec;
    bool m_applyToAll  = false;
    bool m_isReplaced = false;
    JobType m_jobType;
    int m_windowId = -1;

    int m_filedes[2] = {0, 0};
    bool m_isInSameDisk = true;
    bool m_isFinished = false;

    bool copyFile(const QString &srcFile, const QString &tarDir, bool isMoved=false, QString *targetPath = 0);
    bool copyDir(const QString &srcPath, const QString &tarPath, bool isMoved=false, QString *targetPath = 0);
    bool moveFile(const QString &srcFile, const QString &tarDir, QString *targetPath = 0);
    bool restoreTrashFile(const QString &srcFile, const QString &tarFile);
    bool moveDir(const QString &srcFile, const QString &tarDir, QString *targetPath = 0);
    bool deleteFile(const QString &file);
    bool deleteDir(const QString &dir);
    bool moveDirToTrash(const QString &dir, QString *targetPath = 0);
    bool moveFileToTrash(const QString &file, QString *targetPath = 0);
    bool writeTrashInfo(const QString &fileBaseName, const QString &path, const QString &time);

    QString getNotExistsTrashFileName(const QString &fileName);
};

#endif // FILEJOB_H
