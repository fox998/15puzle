import QtQuick 2.0
import Game 1.0

GridView {
    id: root
	model: GameBoardModel{
	}

	cellHeight: height / root.model.dimension
	cellWidth: width / root.model.dimension

    delegate: Item {
        id: _titleBackground

        width: root.cellWidth
        height: root.cellHeight

		visible: Number(display) !== Number(root.model.hidenElement)

        Tile{
			displayText: display.toString()

			anchors.fill: _titleBackground
            anchors.margins: 5

			MouseArea{
				anchors.fill: parent
				onClicked: {
					root.model.move(index);
				}
			}
        }
    }
}
