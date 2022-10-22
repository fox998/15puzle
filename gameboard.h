#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QAbstractListModel>

#include <vector>

class GameBoard : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int dimension READ dimension CONSTANT)
    Q_PROPERTY(int hidenElement READ hidenElement CONSTANT)
public:
    using Tile = size_t;
    using Index = int;


    static constexpr Tile defaultPuzzleDimension{5};
    GameBoard(Tile dimension = defaultPuzzleDimension, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE bool move(int index);

    size_t dimension() const;
    Tile hidenElement() const;

    bool isSwapable(Index firstIndex, Index secondIndex) const;
    bool isValidIndex(const Index i) const;
    Index getHidenElementIndex() const;

private:
    void shuffle(const size_t shiftsCount = 1000);

    std::vector<Tile> m_boardItems;
    const size_t m_dimension;
    const Tile m_hidenElement;
};

#endif // GAMEBOARD_H
