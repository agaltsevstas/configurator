#include "arrow.h"
#include "item.h"
#include "scene.h"

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // При нажатии правой кнопки мыши рисуем линию
    if (mouseEvent->button() == Qt::RightButton)
    {
        _line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
        _line->setPen(QPen(Qt::black, 2));
        addItem(_line);
    } 
    // При нажатии средней кнопки мыши идет удаление
    if (mouseEvent->button() == Qt::MiddleButton)
    {
        int i = -1; // Индекс самой первой удаленной связи между элементами
        QGraphicsItem *itemScene = itemAt(mouseEvent->scenePos(), QTransform());

        // При нажатии на сцену выходим
        if (itemScene == nullptr)
            return;
        // Удаление стрелки
        else if (itemScene->type() == Arrow::Type)
        {
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(itemScene);
            i = arrow->getId();

            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            removeItem(arrow);
            _links.remove(i);
            _isChanged = true;
            delete arrow;
        }
        // Удаление элемента и всех стрелок вместе с ним
        else if (itemScene->type() == Item::Type)
        {
            bool isFirst = false;
            Item *item = qgraphicsitem_cast<Item *>(itemScene);
            QString name = item->getName();

            for (auto it = _links.begin(); it != _links.end();)
            {
                if (it->first == name || it->second == name)
                {
                    if (!isFirst)
                    {
                        i = it.key();
                        isFirst = true;
                    }
                    it = _links.erase(it);
                    _isChanged = true;
                }
                else
                    ++it;
            }
            item->removeArrows();
            item->setEnabledButton();
            _y -= 50;
            removeItem(item);
            delete item;
        }
        // Замена номеров связей, начиная с индекса i
        if (!_links.empty() && i >= 0 && i <= _links.size())
        {
            for (auto it = (_links.begin() + i - 1); it != _links.end(); ++i)
            {
                _links[i] = it.value();
                it = _links.erase(it);
            }
        }
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{    
    if (_line != nullptr)
    {
        // Перетаскивание линии
        QLineF newLine(_line->line().p1(), mouseEvent->scenePos());
        _line->setLine(newLine);
    }
    else
    {
        // Перетаскивание элемента
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
    // Обновление сцены
    update(QRectF(0, 0, this->width(), this->height()));
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // При отпускании правой кнопки мыши рисуем стрелку
    if (_line != nullptr && mouseEvent->button() == Qt::RightButton)
    {
        // Проверка начального и конечного положения стрелки
        QList<QGraphicsItem *> startItems = items(_line->line().p1());
        if (startItems.count() && startItems.first() == _line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(_line->line().p2());
        if (endItems.count() && endItems.first() == _line)
            endItems.removeFirst();

        removeItem(_line);
        delete _line;

        // Проверки на попадания положения концов стрелки на элементы
        bool isUintCount = (startItems.count() > 0) && (endItems.count() > 0);
        bool isTypeItem = isUintCount && (startItems.first()->type() == Item::Type) &&
                                         (endItems.first()->type()   == Item::Type);
        if (isTypeItem && startItems.first() != endItems.first())
        {
            bool isMatched = false;
            Item *startItem = qgraphicsitem_cast<Item *>(startItems.first());
            Item *endItem = qgraphicsitem_cast<Item *>(endItems.first());

            // Проверка на совпадение ранее добавленной связи между элементами
            for (const auto& [key, value]: qAsConst((_links)))
            {
                bool isDirectMatched  = key  == startItem->getName() &&
                                        value == endItem->getName();
                bool isReverseMatched = key  == endItem->getName()   &&
                                        value == startItem->getName();
                if (isDirectMatched || isReverseMatched)
                {
                    isMatched = true;
                    break;
                }
            }
            // Добавление стрелки на сцену и добавление связи между элементами
            if (!isMatched)
            {
                Arrow *arrow = new Arrow(_idArrow, startItem, endItem);
                startItem->addArrow(arrow);
                endItem->addArrow(arrow);
                arrow->setZValue(-1);
                addItem(arrow);
                arrow->updatePosition();
                _links[_idArrow++] = qMakePair(startItem->getName(), endItem->getName());
                _isChanged = true;
                emit arrowInserted();
            }
        }
    }

    _line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}



