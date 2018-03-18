#include "map.h"

Map::Map(QObject *parent) : QObject(parent)
{
    for(int i=0; i<6; i++)
        for(int j=0; j<6; j++)
            m_map[i][j] = 0;
    m_carsNumber = 0;
    m_actionCounter = 0;
}

Map::~Map()
{
    if(m_inputActionsFile.is_open())
        m_inputActionsFile.close();
    delete[] m_cars;
}

Map &Map::getInstance()
{
    static Map the_instant;
    return the_instant;
}

bool Map::readMapFile(QString t_mapUrl)
{
    std::string url = t_mapUrl.toStdString();
    int id;
    std::ifstream inputFile(url);
    if(inputFile.is_open()){
        inputFile >> m_carsNumber;
        m_cars = new Car[m_carsNumber];
        for(int i=0; i< m_carsNumber; i++){
            inputFile >> id;
            m_cars[id-1].id = id;
            inputFile >> m_cars[id-1].x;
            inputFile >> m_cars[id-1].y;
            inputFile >> m_cars[id-1].ornt;
            inputFile >> m_cars[id-1].len;
            for(int continuation=0; continuation<m_cars[id-1].len; continuation++){
                if(m_cars[id-1].ornt == 'h')
                    m_map[m_cars[id-1].x-1][m_cars[id-1].y-1+continuation] = id;
                else if(m_cars[id-1].ornt == 'v')
                    m_map[m_cars[id-1].x-1+continuation][m_cars[id-1].y-1] = id;
            }

        }
        inputFile.close();
        return true;
    }
    return false;
}

void Map::readActionFile(QString t_solutionUrl)
{
    std::string url = t_solutionUrl.toStdString();
    if(m_inputActionsFile.is_open())
        m_inputActionsFile.close();
    m_inputActionsFile.open(url);

}

bool Map::nextAction()
{
    m_inputActionsFile >> m_action.id;
    m_inputActionsFile >> m_action.direction;
    m_inputActionsFile >> m_action.len;
        if(isUpdateMapSafe()){
            m_actionCounter++;
            return true;
        }
    return false;

}

int Map::getActionId()
{
    return m_action.id;
}

char Map::getActionDirection()
{
    return m_action.direction;
}

int Map::getActionLen()
{
    return m_action.len;
}

int Map::getCarsNumber()
{
    return m_carsNumber;
}
int Map::getActionCounter()
{
    return m_actionCounter;
}

int Map::getRow(int t_id)
{
    return m_cars[t_id-1].x;
}

int Map::getColumn(int t_id)
{
    return m_cars[t_id-1].y;
}

char Map::getOrnt(int t_id)
{
    return m_cars[t_id-1].ornt;
}

int Map::getLen(int t_id)
{
    return m_cars[t_id-1].len;
}


bool Map::isUpdateMapSafe()
{
    int id = m_action.id;
    int row = m_cars[id-1].x-1;
    int col = m_cars[id-1].y-1;
    int len = m_cars[id-1].len;
    int val = m_action.len;
    char ornt = m_cars[id-1].ornt;
    char dir = m_action.direction;

    if((dir == 'l' && col-val<0) || (dir == 'r' && (col+len-1)+val>5) || (dir == 'u' && row-val<0)|| (dir == 'd' && (row+len-1)+val>5))
        return false;// out of array

    if(ornt == 'h'){
        if(dir == 'l'){
            for(int i=col-val; i<col; i++)
                if(m_map[row][i] != 0)
                    return false;
            for(int i=col; i<=col+len-1; i++)
                m_map[row][i] = 0;
            for(int i=col-val; i-(col-val)<len; i++)
                m_map[row][i] = id;
            m_cars[id-1].y = col-val+1;
        }
        else if(dir == 'r'){
            for(int i=(col+len-1)+val; i>(col+len-1); i--)
                if(m_map[row][i] != 0)
                    return false;
            for(int i=col; i<=col+len-1; i++)
                m_map[row][i] = 0;
            for(int i=col+val; i-(col+val)<len; i++)
                m_map[row][i] = id;
            m_cars[id-1].y = col+val+1;
        }
    }
    else if(ornt == 'v'){
        if(dir == 'u'){
            for(int i=row-val; i<row; i++)
                if(m_map[i][col] != 0)
                    return false;
            for(int i=row; i<=row+len-1; i++)
                m_map[i][col] = 0;
            for(int i=row-val; i-(row-val)<len; i++)
                m_map[i][col] = id;
            m_cars[id-1].x = row-val+1;
        }
        else if(dir == 'd'){
            for(int i=(row+len-1)+val; i>(row+len-1); i--)
                if(m_map[i][col] != 0)
                    return false;
            for(int i=row; i<=row+len-1; i++)
                m_map[i][col] = 0;
            for(int i=row+val; i-(row+val)<len; i++)
                m_map[i][col] = id;
            m_cars[id-1].x = row+val+1;
        }
    }

    return true;
}
