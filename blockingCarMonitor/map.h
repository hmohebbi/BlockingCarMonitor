#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QString>
#include <fstream>

class Map : public QObject
{
    Q_OBJECT
public:
    ~Map();
    static Map& getInstance();

    Q_INVOKABLE bool readMapFile(QString t_mapUrl);
    Q_INVOKABLE void readActionFile(QString t_solutionUrl);
    Q_INVOKABLE bool nextAction();
    Q_INVOKABLE int getActionId();
    Q_INVOKABLE char getActionDirection();
    Q_INVOKABLE int getActionLen();
    Q_INVOKABLE int getRow(int t_id);
    Q_INVOKABLE int getColumn(int t_id);
    Q_INVOKABLE char getOrnt(int t_id);
    Q_INVOKABLE int getLen(int t_id);
    Q_INVOKABLE int getCarsNumber();
    Q_INVOKABLE int getActionCounter();

    bool isUpdateMapSafe();

signals:

public slots:

private:
    explicit Map(QObject *parent = nullptr);

    struct Car
    {
        int id,x,y,len;
        char ornt;
    };
    struct Action
    {
        int id;
        char direction;
        int len;
    };
    Car *m_cars;
    Action m_action;
    int m_map[6][6];
    std::ifstream m_inputActionsFile;
    int m_carsNumber;
    int m_actionCounter;
};

#endif // MAP_H
