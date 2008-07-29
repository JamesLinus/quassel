/***************************************************************************
 *   Copyright (C) 2005-08 by the Quassel Project                          *
 *   devel@quassel-irc.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) version 3.                                           *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef CHATITEM_H_
#define CHATITEM_H_

#include <QGraphicsItem>
#include <QObject>

#include "chatline.h"
#include "chatlinemodel.h"
#include "chatscene.h"
#include "uistyle.h"

class QTextLayout;

class ChatItem : public QGraphicsItem {

  public:
    ChatItem(const QPersistentModelIndex &index, QGraphicsItem *parent);
    virtual ~ChatItem();

    inline QPersistentModelIndex index() const { return _index; }
    inline const MessageModel *model() const { return _index.isValid() ? qobject_cast<const MessageModel *>(_index.model()) : 0; }
    inline int row() const { return _index.isValid() ? _index.row() : 0; }
    inline ChatScene *chatScene() const { return qobject_cast<ChatScene *>(scene()); }

    inline QFontMetricsF *fontMetrics() const { return _fontMetrics; }
    inline virtual QRectF boundingRect() const { return _boundingRect; }
    inline qreal width() const { return _boundingRect.width(); }
    inline qreal height() const { return _boundingRect.height(); }

    inline bool haveLayout() const { return _layout != 0; }
    void clearLayout();
    void updateLayout();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    virtual QVariant data(int role) const;

    // returns height
    qreal setWidth(qreal width);

    // selection stuff, to be called by the scene
    void clearSelection();
    void setFullSelection();
    void continueSelecting();

  protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

  private:
    qint16 posToCursor(const QPointF &pos);
    qreal computeHeight();
    QTextLayout *createLayout(QTextOption::WrapMode, Qt::Alignment = Qt::AlignLeft);

    // internal selection stuff
    void setSelection(int start, int length);

    QRectF _boundingRect;
    QFontMetricsF *_fontMetrics;
    quint8 _lines;
    QPersistentModelIndex _index;

    QTextLayout * _layout;
    QList<quint16> _wrapPositions;

    enum SelectionMode { NoSelection, PartialSelection, FullSelection };
    SelectionMode _selectionMode;
    qint16 _selectionStart, _selectionEnd;

    class WrapColumnFinder;
};

class ChatItem::WrapColumnFinder {
  public:
    WrapColumnFinder(ChatItem *parent);
    ~WrapColumnFinder();

    qint16 nextWrapColumn();

  private:
    ChatItem *item;
    QTextLayout *layout;
    QTextLine line;
    ChatLineModel::WrapList wrapList;
    qint16 wordidx;
    qint16 lastwrapcol;
    qreal lastwrappos;
    qreal w;
};

#endif
