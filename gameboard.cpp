#include "gameboard.h"
#include <algorithm>
#include <QtGlobal>
#include <QDebug>
#include <chrono>
#include <random>
#include <time.h>

GameBoard::GameBoard(Tile dimension, QObject* parent)
    : QAbstractListModel(parent)
    , m_dimension(dimension)
    , m_hidenElement(dimension * dimension)
{
    m_boardItems.resize(dimension * dimension);
    std::generate(m_boardItems.begin(), m_boardItems.end(), [](){
        static size_t value = 0;
        return ++value;
    });

    shuffle();
}

int GameBoard::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_boardItems.size();
}

QVariant GameBoard::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole || m_boardItems.size() <= index.row())
    {
        qWarning() << "Wrong data() params: " << index << ", " << role;
        return {};
    }


    return QVariant::fromValue(m_boardItems[index.row()]);
}

void GameBoard::shuffle(const size_t shiftsCount)
{
    srand(time(0));
    Index hidenIndex = getHidenElementIndex();

    for (int i = 0; i < shiftsCount;)
    {
        const size_t power = rand() % 2;
        const int sign = 1 -  2 * (rand() % 2);
        const int shiftDelta = std::pow(m_dimension, power) * sign;
        const Index shitedIndex = hidenIndex + shiftDelta;

        if (!isValidIndex(shitedIndex) ||
            !isSwapable(hidenIndex, shitedIndex))
        {
            continue;
        }

        std::swap(m_boardItems[shitedIndex], m_boardItems[hidenIndex]);
        hidenIndex = shitedIndex;
        ++i;
    }
}

size_t GameBoard::hidenElement() const
{
    return m_hidenElement;
}

GameBoard::Index GameBoard::getHidenElementIndex() const
{
    const auto hiden = std::find(m_boardItems.begin(), m_boardItems.end(), m_hidenElement);
    Q_ASSERT(hiden != m_boardItems.cend() && "Can`t find hiden element");
    const Index hidenIndex = std::distance(m_boardItems.begin(), hiden);

    return hidenIndex;
}

bool GameBoard::isSwapable(int firstIndex, int secondIndex) const
{
    if (!isValidIndex(firstIndex) || !isValidIndex(secondIndex))
    {
        return false;
    }

    const size_t distance = std::abs(firstIndex - secondIndex);

    const size_t firstRow = firstIndex / m_dimension;
    const size_t secondRow = secondIndex / m_dimension;
    const bool canSwapHorizontal = (distance == 1 && firstRow == secondRow);
    const bool canSwapVertical = distance == m_dimension;

    return canSwapVertical || canSwapHorizontal;
}

bool GameBoard::move(int index)
{
    if (!isValidIndex(index)) {
        return false;
    }

    const Index hidenIndex = getHidenElementIndex();
    if (!isSwapable(hidenIndex, index))
    {
        return false;
    }

    std::swap(m_boardItems[hidenIndex], m_boardItems[index]);
    emit dataChanged(
                createIndex(std::min(index, hidenIndex), 0),
                createIndex(std::max(index, hidenIndex), 0)
                );
    return true;
}

size_t GameBoard::dimension() const
{
    return m_dimension;
}

bool GameBoard::isValidIndex(const GameBoard::Index i) const
{
    return i >= 0 && i < m_boardItems.size();
}
