#ifndef BINARYOPTIONSMODEL_H
#define BINARYOPTIONSMODEL_H

#include <QAbstractListModel>


class BinaryOptionsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    BinaryOptionsModel(QObject *parent = nullptr);
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QHash<int,QByteArray> roleNames() const override;
};

#endif // BINARYOPTIONSMODEL_H
