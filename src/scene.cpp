#include "scene.h"

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // При нажатии правой кнопки мыши рисуем линию
    if (mouseEvent->button() == Qt::RightButton)
    {
        line_ = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
        line_->setPen(QPen(Qt::black, 2));
        addItem(line_);
    } 
    // При нажатии средней кнопки мыши идет удаление
    if (mouseEvent->button() == Qt::MidButton)
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
            links_.remove(i);
            isChanged_ = true;
            delete arrow;
        }
        // Удаление элемента и всех стрелок вместе с ним
        else if (itemScene->type() == Item::Type)
        {
            bool isFirst = false;
            Item *item = qgraphicsitem_cast<Item *>(itemScene);
            QString name = item->getName();

            for (auto it = links_.begin(); it != links_.end();)
            {
                if (it->first == name || it->second == name)
                {
                    if (!isFirst)
                    {
                        i = it.key();
                        isFirst = true;
                    }
                    it = links_.erase(it);
                    isChanged_ = true;
                }
                else
                    ++it;
            }
            item->removeArrows();
            item->setEnabledButton();
            y_ -= 40;
            removeItem(item);
            delete item;
        }
        // Замена номеров связей, начиная с индекса i
        if (!links_.empty() && i >= 0 && i <= links_.size())
        {
            for (auto it = (links_.begin() + i - 1); it != links_.end(); ++i)
            {
                links_[i] = it.value();
                it = links_.erase(it);
            }
        }
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{    
    if (line_ != nullptr)
    {
        // Перетаскивание линии
        QLineF newLine(line_->line().p1(), mouseEvent->scenePos());
        line_->setLine(newLine);
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
    if (line_ != nullptr && mouseEvent->button() == Qt::RightButton)
    {
        // Проверка начального и конечного положения стрелки
        QList<QGraphicsItem *> startItems = items(line_->line().p1());
        if (startItems.count() && startItems.first() == line_)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line_->line().p2());
        if (endItems.count() && endItems.first() == line_)
            endItems.removeFirst();

        removeItem(line_);
        delete line_;

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
            for (const auto &pair: links_)
            {
                bool isDirectMatched  = pair.first  == startItem->getName() &&
                                        pair.second == endItem->getName();
                bool isReverseMatched = pair.first  == endItem->getName()   &&
                                        pair.second == startItem->getName();
                if (isDirectMatched || isReverseMatched)
                {
                    isMatched = true;
                    break;
                }
            }
            // Добавление стрелки на сцену и добавление связи между элементами
            if (!isMatched)
            {
                Arrow *arrow = new Arrow(idArrow_, startItem, endItem);
                startItem->addArrow(arrow);
                endItem->addArrow(arrow);
                arrow->setZValue(-1);
                addItem(arrow);
                arrow->updatePosition();
                links_[idArrow_++] = qMakePair(startItem->getName(), endItem->getName());
                isChanged_ = true;
                emit arrowInserted();
            }
        }
    }

    line_ = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}



