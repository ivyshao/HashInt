/*
   Part of previous code, it's A* search algorithm
   A can move distance 3:

   0 0 0 0 0 0 0
   0 0 0 0 0 0 0
   0 0 0 0 0 0 0
   0 0 0 A 0 0 0
   0 0 0 0 0 0 0
   0 0 0 0 0 0 0
   0 0 0 0 0 0 0

   Then the range this character can move is:

   0 0 0 3 0 0 0
   0 0 3 2 3 0 0
   0 3 2 1 2 3 0
   3 2 1 A 1 2 3
   0 3 2 1 2 3 0
   0 0 3 2 3 0 0
   0 0 0 3 0 0 0

   If A want to move to B:

   0 0 0 0 0 0 0
   0 0 B 0 0 0 0
   0 0 0 0 0 0 0
   0 0 0 A 0 0 0
   0 0 0 0 0 0 0
   0 0 0 0 0 0 0
   0 0 0 0 0 0 0

   Then it will move:
   0 0 0 0 0 0 0
   0 0 B < 0 0 0
   0 0 0 ^ 0 0 0
   0 0 0 A 0 0 0
   0 0 0 0 0 0 0
   0 0 0 0 0 0 0
   0 0 0 0 0 0 0
   */

cocos2d::CCArray* WarManager::moveRangeForCharacter(SCharacter* sc, int distance) {
    ARMY_SIDE otherSide = OUR;
    if (sc->characterModel->side == OUR || sc->characterModel->side == FRIEND) {
        otherSide = ENEMY;
    }
    CCArray *openArr = CCArray::create();
    CCArray *closedArr = CCArray::create();
    TileCoord *startCoord = TileCoord::create(sc->pos.x, sc->pos.y);
    startCoord->distance = distance;
    CCArray *path = CCArray::create();
    path->addObject(TileCoord::create(sc->pos.x, sc->pos.y, distance));
    startCoord->setPath(path);
    openArr->addObject(startCoord);
    while (openArr->count() > 0) {
        CCObject *obj;
        int maxDistance = -1;
        TileCoord *nextCoord;
        CCARRAY_FOREACH(openArr, obj) {
            TileCoord *openCoord = (TileCoord *)obj;
            if (openCoord->distance > maxDistance) {
                maxDistance = openCoord->distance;
                nextCoord = openCoord;
            }
        }
        closedArr->addObject(nextCoord);
        openArr->removeObject(nextCoord);
        this->addMoveRange(sc, nextCoord, closedArr, openArr, otherSide);
    }
    return closedArr;
}

void WarManager::addMoveRange(SCharacter* sc, TileCoord* tilePoint, cocos2d::CCArray* closedArr, cocos2d::CCArray* openArr, ARMY_SIDE otherSide) {
    if (tilePoint->distance <= 0) return;
    int col = tilePoint->col;
    int row = tilePoint->row;

    bool canNotThrough = false;
    if (closedArr->count() != 1) {
        if (this->isOtherSideCharacterAtCoord(col-1, row, otherSide) ||
            this->isOtherSideCharacterAtCoord(col+1, row, otherSide) ||
            this->isOtherSideCharacterAtCoord(col, row-1, otherSide) ||
            this->isOtherSideCharacterAtCoord(col, row+1, otherSide)) {
            canNotThrough = true;
        }
    }

    if (!canNotThrough) {
        if (row-1 >= 0) {
            this->_checkToAddPointInOpenArr(sc, TileCoord::create(col, row-1), closedArr, openArr, otherSide, tilePoint);
        }
        if (col+1 < layerInfo->m_tLayerSize.width) {
            this->_checkToAddPointInOpenArr(sc, TileCoord::create(col+1, row), closedArr, openArr, otherSide, tilePoint);
        }
        if (row+1 < layerInfo->m_tLayerSize.height) {
            this->_checkToAddPointInOpenArr(sc, TileCoord::create(col, row+1), closedArr, openArr, otherSide, tilePoint);
        }
        if (col-1 >= 0) {
            this->_checkToAddPointInOpenArr(sc, TileCoord::create(col-1, row), closedArr, openArr, otherSide, tilePoint);
        }
    }

}

void WarManager::_checkToAddPointInOpenArr(SCharacter* sc, TileCoord* tilePoint, cocos2d::CCArray* closedArr, cocos2d::CCArray* openArr, ARMY_SIDE otherSide, TileCoord* parentTile) {
    // If point in closed array, no need to handle
    CCObject *obj;
    CCARRAY_FOREACH(closedArr, obj) {
        TileCoord* t = (TileCoord *)obj;
        if (t->col == tilePoint->col &&
            t->row == tilePoint->row) {
            return;
        }
    }

    if (this->isOtherSideCharacterAtCoord(tilePoint->col, tilePoint->row, otherSide)) return;
    int index = tilePoint->row*layerInfo->m_tLayerSize.width+tilePoint->col;
    int terrainId = layerInfo->m_pTiles[index];
    int mov = sc->characterModel->getMovForTerrain(terrainId);
    int distance = parentTile->distance;
    distance -= mov;
    if (distance < 0) return;

    TileCoord *tmp = tilePoint;

    bool inOpened = false;
    CCARRAY_FOREACH(openArr, obj) {
        TileCoord* t = (TileCoord *)obj;
        if (t->col == tilePoint->col &&
            t->row == tilePoint->row) {
            inOpened = true;
            tmp = t;
            break;
        }
    }
    if (!inOpened) {
        openArr->addObject(tilePoint);
    }

    if (!tmp->path || tmp->distance < distance) {
        tmp->distance = distance;
        CCArray * pathCopy = (CCArray*)parentTile->path->copy();
        pathCopy->addObject(TileCoord::create(tilePoint->col, tilePoint->row, distance));
        tmp->setPath(pathCopy);
        CC_SAFE_RELEASE(pathCopy);
    }
}

