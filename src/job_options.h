#pragma once
#include <QListWidget>
#include <qexception.h>
#include <quuid.h>

class JobOptions {
public:
  explicit JobOptions(bool isDownload);
  JobOptions();

  ~JobOptions();

  enum Operation { UnknownOp, Copy, Move, Sync, Mount, Check, CryptCheck };
  enum JobType { UnknownJobType, Upload, Download };

  /*
   * The following enums have their int values synchronized with the
   * list indexes on the gui.  Changes needed to be synchronized.
   */
  enum MountCacheLevel { Off, Minimal, Writes, Full, UnknownCacheLevel };
  enum SyncTiming { During, After, Before, UnknownTiming };
  enum CompareOption {
    SizeAndModTime,
    Checksum,
    IgnoreSize,
    SizeOnly,
    ChecksumIgnoreSize
  };

  QString description;

  JobType jobType;
  Operation operation;
  bool dryRun; // not persisted
  bool sync;
  SyncTiming syncTiming;
  bool skipNewer;
  bool skipExisting;
  bool compare;
  CompareOption compareOption;
  bool verbose;
  bool sameFilesystem;
  bool dontUpdateModified;
  QString transfers;
  QString checkers;
  QString bandwidth;
  QString minSize;
  QString minAge;
  QString maxAge;
  int maxDepth;
  QString connectTimeout;
  QString idleTimeout;
  QString retries;
  QString lowLevelRetries;
  bool deleteExcluded;
  QString excluded;
  QString extra;
  QString source;
  QString dest;
  bool isFolder;
  QUuid uniqueId;
  bool DriveSharedWithMe;
  QString remoteMode;
  QString remoteType;

  // added for mount task
  bool mountReadOnly;
  MountCacheLevel mountCacheLevel;
  QString mountVolume;
  bool mountAutoStart;
  QString mountRcPort;
  QString mountScript;
  bool mountWinDriveMode;

  void setJobType(bool isDownload) {
    jobType = (isDownload) ? Download : Upload;
  }

  QString myName() const {
    return "JobOptions"; // this->staticQtMetaObject.myName();
  }
  QStringList getOptions() const;

  bool operator==(const JobOptions &other) const {
    return uniqueId == other.uniqueId;
  }

  /*
   * This allows the de-serialization method to accomodate changes
   * to the class structure, especially (most easily) added members.
   *
   * Increment the value each time a change is made, emit the new field(s)
   * in the operator<< function, and in operator>> add conditional logic
   * based on the version for reading in the new field(s)
   */
  static const qint32 classVersion;
};

class JobOptionsListWidgetItem : public QListWidgetItem {
public:
  JobOptionsListWidgetItem(JobOptions *jo, const QIcon &icon,
                           const QString &text, const QString &requestId)
      : QListWidgetItem(icon, text), mJobData(jo), mRequestId(requestId) {}

  void SetData(JobOptions *jo) { mJobData = jo; }
  JobOptions *GetData() { return mJobData; }
  QString GetRequestId() { return mRequestId; }

private:
  JobOptions *mJobData;
  QString mRequestId;
};

class SerializationException : public QException {
public:
  QString Message;
  explicit SerializationException(QString msg);
};
