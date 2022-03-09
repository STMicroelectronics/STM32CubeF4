/**
  ******************************************************************************
  * @file    game_win.c
  * @author  MCD Application Team
  * @brief   game functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "DIALOG.h"
#include "k_module.h"
#include "game_res.c"

/** @addtogroup GAME_MODULE
  * @{
  */

/** @defgroup GAME
  * @brief game routines
  * @{
  */
  
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  game_board =
{
  2,
  "Game",
  &bmgame,
  Startup,
  NULL,
}
;

/* Private defines -----------------------------------------------------------*/
#define ID_FRAMEWIN_INFO        (GUI_ID_USER + 0x01)
#define ID_IMAGE_INFO           (GUI_ID_USER + 0x02)
#define ID_BUTTON_INFO_CLOSE    (GUI_ID_USER + 0x03)

#define NUM_CELL_X        8
#define NUM_CELL_Y        8

#define AA_USE_HIRES      0
#define AA_FACTOR         1

#if AA_USE_HIRES
  #define AA_CALCFACTOR   AA_FACTOR
#else
  #define AA_CALCFACTOR   1
#endif

#define CLIENT_COLOR      GUI_TRANSPARENT
#define GRID_COLOR        GUI_BROWN

/* Defines, player AI 'SmartGecko' */
#define DEPTH          4
#define END_GAME_DEPTH 9

#define INFINITY       1000000
#define WINNING_BONUS   100000

#define VALUE_OF_A_MOVE_POSSIBILITY 15
#define VALUE_OF_AN_UNSAFE_PIECE     8
#define VALUE_OF_A_SAFE_PIECE       20
#define VALUE_OF_A_CORNER         1000

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef struct {
  U8  aCells[NUM_CELL_X][NUM_CELL_Y];
  U8  aMoves[NUM_CELL_X][NUM_CELL_Y];
  int ActPlayer;
} BOARD;

typedef char REVERSI_AI_Func(const BOARD * pBoard, int* px, int* py);
static WM_HWIN          _hFrame;
static REVERSI_AI_Func* _pPlayerAI[2];
static BOARD            _Board;
static int              _GameOver;
static int              _BoardX0;
static int              _BoardY0;
static int              _CellSize;
/* Game settings */
static int              _ShowPossibleMoves = 1;
static int * _px;
static int * _py;
static BOARD _aBoardStack[END_GAME_DEPTH + 1];
static const int _xs[60] = { 
  7, 7, 0, 0, 7, 7, 5, 5, 2, 2, 
  0, 0, 5, 5, 2, 2, 5, 5, 4, 4, 
  3, 3, 2, 2, 7, 7, 4, 4, 3, 3, 
  0, 0, 6, 6, 6, 6, 5, 5, 4, 4, 
  3, 3, 2, 2, 1, 1, 1, 1, 7, 7, 
  6, 6, 1, 1, 0, 0, 6, 6, 1, 1
};
static const int _ys[60] = { 
  7, 0, 7, 0, 5, 2, 7, 0, 7, 0, 
  5, 2, 5, 2, 5, 2, 4, 3, 5, 2, 
  5, 2, 4, 3, 4, 3, 7, 0, 7, 0, 
  4, 3, 5, 4, 3, 2, 6, 1, 6, 1, 
  6, 1, 6, 1, 5, 4, 3, 2, 6, 1, 
  7, 0, 7, 0, 6, 1, 6, 1, 6, 1 
};
static int _aaSafe[10][10];
static const I32 _aaValues[8][8] = { 
  { 1000, -100, 25,  0,  0, 25, -100, 1000 },
  { -100, -400, -5, -5, -5, -5, -400, -100 },
  {   25,   -5, 12,  2,  2, 12,   -5,   25 },
  {    0,   -5,  2,  2,  2,  2,   -5,    0 },
  {    0,   -5,  2,  2,  2,  2,   -5,    0 },
  {   25,   -5, 12,  2,  2, 12,   -5,   25 },
  { -100, -400, -5, -5, -5, -5, -400, -100 },
  { 1000, -100, 25,  0,  0, 25, -100, 1000 }
};

/* Private functions ---------------------------------------------------------*/
static void _StartNewGame(void);

/* Calculate Board Dimensions */
static void _CalcBoardDimensions(void) {
  GUI_RECT r;
  WM_GetClientRectEx(WM_GetClientWindow(_hFrame), &r);
  _CellSize  = ((r.x1 > r.y1) ? r.y1 : r.x1) >> 3;
  _BoardX0   = (r.x1 - (_CellSize << 3)) >> 1;
  _BoardY0   = (r.y1 - (_CellSize << 3)) >> 1;
}

/* Invalidate Board */
static void _InvalidateBoard(void) {
  WM_InvalidateWindow(WM_GetClientWindow(_hFrame));
}

/* Invalidate Cell */
static void _InvalidateCell(int x, int y) {
  GUI_RECT r;
  r.x0 = _BoardX0 + (x * _CellSize);
  r.y0 = _BoardY0 + (y * _CellSize);
  r.x1 = r.x0 + _CellSize - 1;
  r.y1 = r.y0 + _CellSize - 1;
  WM_InvalidateRect(WM_GetClientWindow(_hFrame), &r);
}

/**
  * @brief  Get Stone
  * @param  pBoard: pointer to the board stack
  * @param  x: X position     
  * @param  y: Y position
  * @retval stone
  */
static char _GetStone(const BOARD * pBoard, int x, int y) {
  char r = 0;
  if ((x >= 0) && (y >= 0) && (x < NUM_CELL_X) && (y < NUM_CELL_Y)) {
    r = pBoard->aCells[x][y];
  }
  return r;
}

/**
  * @brief  Set Stone
  * @param  pBoard: pointer to the board stack
  * @param  x: X position     
  * @param  y: Y position
  * @retval None
  */
static void _SetStone(BOARD * pBoard, int x, int y) {
  if ((x >= 0) && (y >= 0) && (x < NUM_CELL_X) && (y < NUM_CELL_Y)) {
    pBoard->aCells[x][y] = pBoard->ActPlayer;
    _InvalidateCell(x, y);
  }
}

/**
  * @brief  Is Valid Move
  * @param  pBoard: pointer to the board stack
  * @param  x: X position     
  * @param  y: Y position
  * @retval status
  */
static char _IsValidMove(BOARD * pBoard, int x, int y) {
  char r = 0;
  if ((x >= 0) && (y >= 0) && (x < NUM_CELL_X) && (y < NUM_CELL_Y)) {
    r = ((pBoard->aMoves[x][y]) ? 1 : 0);
  }
  return r;
}

/**
  * @brief  Check Direction
  * @param  pBoard: pointer to the board stack
  * @param  x: X position     
  * @param  y: Y position
  * @param  dx: difference between current and previous X position
  * @param  dy: difference between current and previous Y position
  * @retval status
  */
static char _CheckDirection(const BOARD * pBoard, int x, int y, int dx, int dy) {
  char Cell;
  x += dx;
  y += dy;
  Cell = _GetStone(pBoard, x, y);
  if ((Cell != pBoard->ActPlayer) && (Cell != 0)) {
    do {
      x += dx;
      y += dy;
      Cell = _GetStone(pBoard, x, y);
    } while ((Cell != pBoard->ActPlayer) && (Cell != 0));
    return ((Cell == pBoard->ActPlayer) ? 1 : 0);
  }
  return 0;
}

/**
  * @brief  Calculate valid moves
  * @param  pBoard: pointer to the board stack
  * @retval valid moves
  */
static int _CalcValidMoves(BOARD * pBoard) {
  int x, y, r = 0;
  U8 Valid;
  for (y = 0; y < NUM_CELL_Y; y++) {
    for (x = 0; x < NUM_CELL_X; x++) {
      Valid = 0;
      if (pBoard->aCells[x][y] == 0) {
        Valid |= _CheckDirection(pBoard, x, y, -1, -1) << 0;
        Valid |= _CheckDirection(pBoard, x, y,  0, -1) << 1;
        Valid |= _CheckDirection(pBoard, x, y,  1, -1) << 2;
        Valid |= _CheckDirection(pBoard, x, y,  1,  0) << 3;
        Valid |= _CheckDirection(pBoard, x, y,  1,  1) << 4;
        Valid |= _CheckDirection(pBoard, x, y,  0,  1) << 5;
        Valid |= _CheckDirection(pBoard, x, y, -1,  1) << 6;
        Valid |= _CheckDirection(pBoard, x, y, -1,  0) << 7;
        if (Valid) {
          r++;
        }
      }
      if (Valid != pBoard->aMoves[x][y]) {
        pBoard->aMoves[x][y] = Valid;
        _InvalidateCell(x, y);
      }
    }
  }
  return r;
}

/**
  * @brief  Do Direction
  * @param  pBoard: pointer to the board stack
  * @param  x: X position     
  * @param  y: Y position
  * @param  dx: difference between current and previous X position
  * @param  dy: difference between current and previous Y position
  * @retval None
  */ 
static void _DoDirection(BOARD * pBoard, int x, int y, int dx, int dy) {
  do {
    _SetStone(pBoard, x, y);
    x += dx;
    y += dy;
  } while (_GetStone(pBoard, x, y) != pBoard->ActPlayer);
}

/**
  * @brief  Make Move
  * @param  pBoard: pointer to the board stack
  * @param  x: X position     
  * @param  y: Y position
  * @retval None
  */ 
static void _MakeMove(BOARD * pBoard, int x, int y) {
  U8 Valid;
  _SetStone(pBoard, x, y);
  Valid = pBoard->aMoves[x][y];
  if (Valid & (U8)(1 << 0)) { _DoDirection(pBoard, x, y, -1, -1); }
  if (Valid & (U8)(1 << 1)) { _DoDirection(pBoard, x, y,  0, -1); }
  if (Valid & (U8)(1 << 2)) { _DoDirection(pBoard, x, y,  1, -1); }
  if (Valid & (U8)(1 << 3)) { _DoDirection(pBoard, x, y,  1,  0); }
  if (Valid & (U8)(1 << 4)) { _DoDirection(pBoard, x, y,  1,  1); }
  if (Valid & (U8)(1 << 5)) { _DoDirection(pBoard, x, y,  0,  1); }
  if (Valid & (U8)(1 << 6)) { _DoDirection(pBoard, x, y, -1,  1); }
  if (Valid & (U8)(1 << 7)) { _DoDirection(pBoard, x, y, -1,  0); }
}

/**
  * @brief  Calculate Score
  * @param  pBoard: pointer to the board stack
  * @retval Score
  */ 
static int _CalcScore(const BOARD * pBoard) {
  int x, y, r = 0;
  char Cell;
  for (y = 0; y < NUM_CELL_Y; y++) {
    for (x = 0; x < NUM_CELL_X; x++) {
      Cell = pBoard->aCells[x][y];
      if (Cell) {
        r += (Cell == pBoard->ActPlayer) ? (1) : (-1);
      }
    }
  }
  return r;
}


/**
  * @brief Find the value of all the pieces belonging to the given player.
  * @note  Positive value is good for this player.
  *        The value can also be negative, if the player occupies tiles
  *        next to free corner, which makes it easier for the opponent
  *        to get to this corner.
  * @param  pBoard: pointer to the board stack
  * @param  player: player index
  * @retval value
  */ 
static I32 _valuePieces(BOARD * pBoard, int player) {
  I32 sum;
  int x, y, corners, changed, s, xCorner, yCorner, numberOfSafe;
  
  sum = 0;
  for (y = 1; y <= 8; y++) {
    for (x = 1; x <= 8; x++) {
      _aaSafe[x][y] = 0;
    }
  }
  corners = 0;
  corners += pBoard->aCells[0][0] == player;
  corners += pBoard->aCells[7][0] == player;
  corners += pBoard->aCells[0][7] == player;
  corners += pBoard->aCells[7][7] == player;
  if (corners) {
    /* Corners are the most valuable asset of the position. */
    sum += corners * VALUE_OF_A_CORNER;
    /* Calculate how many pieces are safe,
       meaning they can never be taken back
       by the opponent. */
    while (1) {
      /* Repeatedly swipe the board looking for safe pieces,
         until no more safe pieces are found. */
      changed = 0;
      numberOfSafe = 0;
      for (y = 1; y <= 8; y++) {
        for (x = 1; x <= 8; x++) {
          if (!_aaSafe[x][y] && pBoard->aCells[x - 1][y - 1] == player) {
            /* A piece can never be taken back if in each of the four directions,
               one of the two neighboring tiles are safe. */
            s =  (_aaSafe[x - 1][y    ] || _aaSafe[x + 1][y    ])  /* West  - East  */
              && (_aaSafe[x    ][y - 1] || _aaSafe[x    ][y + 1])  /* North - South */
              && (_aaSafe[x - 1][y - 1] || _aaSafe[x + 1][y + 1])  /* NW    - SE    */
              && (_aaSafe[x - 1][y + 1] || _aaSafe[x + 1][y - 1]); /* SW    - NE    */
            if (s) {
              _aaSafe[x][y] = 1;
              changed = 1;
              ++numberOfSafe;
            }
          }
        }
        sum += numberOfSafe * VALUE_OF_A_SAFE_PIECE;
      }
      if (!changed) {
        break;
      }
    }
  }
  /* Now add the value of the unsafe pieces. */
  for (y = 0; y < 8; y++) {
    yCorner = (y < 4) ? 0 : 7;
    for (x = 0; x < 8; x++) {
      if (pBoard->aCells[x][y] == player && !_aaSafe[x + 1][y + 1]) {
        xCorner = x<4 ? 0 : 7;
        if (pBoard->aCells[xCorner][yCorner]) {
          /* If the corner is taken, we value each position in
             the quadrant the same. */
          sum += VALUE_OF_AN_UNSAFE_PIECE;
        } else {
          /* If the corner is still free, we use a lookup table
             to find the value of each position. */
          sum += _aaValues[x][y];
        }
      }
    }
  }
  return sum;
}
/**
  * @brief Evaluation function for Reversi board.
  * @note  Positive value means player 1 is in the lead,
  *        negative value means player 2 is in the lead.
  * @param  pBoard: pointer to the board stack
  * @retval value
  */ 

static I32 _Eval(BOARD * pBoard) {
  int ActPlayer, movesA, movesB;
  I32 score, value;

  ActPlayer = pBoard->ActPlayer;
  pBoard->ActPlayer = 1;
  movesA = _CalcValidMoves(pBoard);
  pBoard->ActPlayer = 2;
  movesB = _CalcValidMoves(pBoard);
  pBoard->ActPlayer = ActPlayer;
  if (movesA == 0 && movesB == 0) {
    /* The game is over */
    pBoard->ActPlayer = 1;
    score = _CalcScore(pBoard);
    pBoard->ActPlayer = ActPlayer;
    if (score==0) {
      return 0;
    }
    if (score > 0) {
      return score + WINNING_BONUS;
    }
    if (score > 0) {
      return score - WINNING_BONUS;
    }
  }
  /* High number of possible moves is very valuable */
  value  = VALUE_OF_A_MOVE_POSSIBILITY * (movesA - movesB);
  value += _valuePieces(pBoard, 1);
  value -= _valuePieces(pBoard, 2);
  return value;
}


/**
  * @brief Minimax search for the best possible move with alpha-beta pruning
  * @param  depth: board depth
  * @param  alpha: current Move
  * @param  beta:  previous Move
  * @param  firstMove: first Move index
  * @retval best move
  */ 
static I32 _Descend(int depth, I32 alpha, I32 beta, int firstMove) {
  BOARD * pBoard;
  BOARD * nextBoard;
  int x, y, moves, i, alt, maximize;

  pBoard    = _aBoardStack + depth;
  nextBoard = _aBoardStack + depth - 1;
  if (depth == 0) {
    return _Eval(pBoard);
  }
  moves = _CalcValidMoves(pBoard);
  if (moves == 0) {
      /* The player has to pass */
      pBoard->ActPlayer = 3 - pBoard->ActPlayer;
      moves = _CalcValidMoves(pBoard);
      if (moves == 0) {
        /* The game is over */
        return _Eval(pBoard);
      }
  }
  maximize = pBoard->ActPlayer == 1;
  for (i = 0; i < 60; ++i) {
    /* Try the possible moves in order from most attractive to least attractive
       position, to maximize the effect of the alpha-beta pruning. */
    x = _xs[i];
    y = _ys[i];
    if (pBoard->aMoves[x][y]) {
      *nextBoard = *pBoard;
      _MakeMove(nextBoard, x, y);
      nextBoard->ActPlayer = 3 - pBoard->ActPlayer;
      /* Recursively evaluate the board resulting from this move. */
      alt = _Descend(depth - 1, alpha, beta, 0);
      if (maximize) {
        if (alt > alpha) {
          alpha = alt;
          if (firstMove) {
            *_px = x;
            *_py = y;
          }
        }
      } else {
        if (alt < beta) {
          beta = alt;
          if (firstMove) {
            *_px = x;
            *_py = y;
          }
        }
      }
      if (beta <= alpha) {
        /* This is the alpha-beta pruning terminal condition. */
        goto end;
      }
    }
  }
end:
  return maximize ? alpha : beta;
}

/**
  * @brief _PlayerAI_SmartGecko
  * @param  pBoard: pointer to the board stack
  * @param  x: X position     
  * @param  y: Y position
  * @retval char
  */ 
static char _PlayerAI_SmartGecko(const BOARD * pBoard, int * px, int * py) {
  int x, y, freeTiles, depth;

  /* Initialize the safe board to true to get the edges right */
  for (y = 0; y < 10; y++) {
    for (x = 0; x < 10; x++) {
      _aaSafe[x][y] = 1;
    }
  }
  *px = -1;
  *py = -1;
  _px = px;
  _py = py;
  freeTiles = 0;
  for (y = 0; y < 8; y++) {
    for (x = 0; x < 8; x++) {
      if (!pBoard->aCells[x][y]) {
        ++freeTiles;
      }
    }
  }
  depth = DEPTH;
  if (freeTiles <= END_GAME_DEPTH) {
    /* In the end game, we expand the search depth. */
    depth = freeTiles;
  }
  _aBoardStack[depth] = *pBoard;
  _Descend(depth, -INFINITY, INFINITY, 1);
  if (*px == -1) {
    return 0;
  }
  return 1;
}

/**
  * @brief callback Message Box 
  * @param  pMsg : pointer to data structure
  * @retval None
  */
static void _cbMessageBox(WM_MESSAGE* pMsg) {
  WM_HWIN hWin;
  int Id;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
      Id = WM_GetId(pMsg->hWinSrc);
       GUI_EndDialog(hWin, (Id == GUI_ID_OK) ? 1 : 0);
    }
    
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Show Message Box
  * @param  pTitle: pointer to the title
  * @param  pText: pointer to the text
  * @param  YesNo player response
  * @retval int 
  */ 
static int _ShowMessageBox(WM_HWIN hWin, const char* pTitle, const char* pText, int YesNo) {
  WM_HWIN hFrame, hClient, hBut;
  int r = 0;
  /* Create frame win */
  hFrame = FRAMEWIN_CreateEx(225, 180, 190, 120, hWin, WM_CF_SHOW, FRAMEWIN_CF_MOVEABLE, 0, pTitle, &_cbMessageBox);
  FRAMEWIN_SetClientColor   (hFrame, GUI_WHITE);
  FRAMEWIN_SetFont          (hFrame, &GUI_Font16B_ASCII);
  FRAMEWIN_SetTextAlign     (hFrame, GUI_TA_HCENTER);
  /* Create dialog items */
  hClient = WM_GetClientWindow(hFrame);
  TEXT_CreateEx(10, 7, 170, 30, hClient, WM_CF_SHOW, GUI_TA_HCENTER, 0, pText);
  if (YesNo) {
    hBut = BUTTON_CreateEx(97, 45, 55, 18, hClient, WM_CF_SHOW, 0, GUI_ID_CANCEL);
    BUTTON_SetText        (hBut, "No");
    hBut = BUTTON_CreateEx(32, 45, 55, 18, hClient, WM_CF_SHOW, 0, GUI_ID_OK);
    BUTTON_SetText        (hBut, "Yes");
  } else {
    hBut = BUTTON_CreateEx(64, 45, 55, 18, hClient, WM_CF_SHOW, 0, GUI_ID_OK);
    BUTTON_SetText        (hBut, "Ok");
  }
  
  WM_SetFocus(_hFrame);  
  WM_MakeModal(hFrame);
  r = GUI_ExecCreatedDialog(hFrame);  
  return r;
}

/**
  * @brief  Set Player
  * @param  Player: player index
  * @retval None 
  */ 
static void _SetPlayer(int Player) {
  int Score, ValidMoves, PossibleMoves;
  char ac[256];
  
  _Board.ActPlayer = Player;
  if (Player == 1) {
    FRAMEWIN_SetText(_hFrame, "Reversi - User");
  } else {
    FRAMEWIN_SetText(_hFrame, "Reversi - CPU");
  }
  FRAMEWIN_SetBarColor(_hFrame, 1, (Player == 1) ? GUI_RED : GUI_BLUE);
  PossibleMoves = _CalcValidMoves(&_Board);
  GUI_Exec();
  
  if (!PossibleMoves) {
    GUI_Exec();
    _Board.ActPlayer = 3 - Player;
    ValidMoves = _CalcValidMoves(&_Board);
    _Board.ActPlayer = Player;
    _CalcValidMoves(&_Board);
    if (ValidMoves) {   /* No valid moves, player must pass */
      if (_pPlayerAI[_Board.ActPlayer - 1] == NULL) {
        _ShowMessageBox(_hFrame, "Reversi", "No possible moves.\nYou have to pass!", 0);
      } else {
        /* Pass ai player */
      }
      _SetPlayer(3 - _Board.ActPlayer);
    } else {            /* No valid moves for all players, game is over */
      _GameOver = 1;
      _Board.ActPlayer = 1;
      Score = _CalcScore(&_Board);
      if (Score > 0) {
        sprintf(ac, "Red wins by %d stones!\nDo you want to start a new game?", Score);
      } else if (Score) {
        sprintf(ac, "Blue wins by %d stones!\nDo you want to start a new game?", -Score);
      } else {
        strcpy(ac, "The game ends in a draw!\nDo you want to start a new game?");
      }
      if (_ShowMessageBox(_hFrame, "Reversi", ac, 1)) {
        _StartNewGame();
      }
    }
  }
}

/**
  * @brief  Next Player 
  * @param  None 
  * @retval None
  */
static void _NextPlayer(void) {
  int x, y;
  
  do {
    _SetPlayer(3 - _Board.ActPlayer);
    if (_pPlayerAI[_Board.ActPlayer - 1]&& !_GameOver) {
      char DoMove;
      DoMove = (*_pPlayerAI[_Board.ActPlayer - 1])(&_Board, &x, &y);
      if (DoMove) {
        _MakeMove(&_Board, x, y);
      }
    }
  } while (_pPlayerAI[_Board.ActPlayer - 1] && !_GameOver);
}

/**
  * @brief  Start new Game 
  * @param  None
  * @retval None
  */
static void _StartNewGame(void) {
  memset(&_Board, 0, sizeof(BOARD));
  _Board.aCells[3][3] = 1;
  _Board.aCells[4][4] = 1;
  _Board.aCells[3][4] = 2;
  _Board.aCells[4][3] = 2;
  _GameOver    = 0;
  _SetPlayer(1);
  _InvalidateBoard();
}

/**
  * @brief Handle PID 
  * @param  x: X position     
  * @param  y: Y position
  * @param Pressed: touch status
  * @retval None
  */
static void _HandlePID(int x, int y, int Pressed) {
  static int _IsInHandlePID;

  if (_IsInHandlePID++ == 0) {
    _CalcBoardDimensions();
    x -= _BoardX0;
    y -= _BoardY0;
    if ((x >= 0) && (y >= 0)) {
      x /= _CellSize;
      y /= _CellSize;
      if ((x < 8) && (y < 8)) {
        if (_IsValidMove(&_Board, x, y)) {
          if (Pressed == 0) {
            _MakeMove(&_Board, x, y);
            _NextPlayer();
          } 
          goto EndHandlePID;
        }   
      }
    }
  }
EndHandlePID:
  _IsInHandlePID--;
}

/**
  * @brief _OnTouch 
  * @param pMsg : pointer to data structure
  * @retval None
  */
static void _OnTouch(WM_MESSAGE* pMsg) {
  const GUI_PID_STATE* pState = (const GUI_PID_STATE*)pMsg->Data.p;
  if (pState) 
  {
    _HandlePID(pState->x, pState->y, pState->Pressed);
  }
}

/**
  * @brief  Paints callback 
  * @param  hWin: pointer to the parent handle
  * @retval None
  */
static void _OnPaint(WM_HWIN hWin) {
  GUI_COLOR Color;
  GUI_RECT  r;
  int x, y, xPos, yPos;
  int CellSize, rStone, rMove;
  char Cell, IsValidMove;
  int xCircle, yCircle;
  
  _CalcBoardDimensions();
  GUI_AA_SetFactor(AA_FACTOR);
  #if AA_USE_HIRES
    GUI_AA_EnableHiRes();
  #endif
  LCD_SetBkColor(CLIENT_COLOR);
  WM_GetClientRectEx(hWin, &r);
  GUI_ClearRect(r.x0, r.y0, r.x1, _BoardY0 - 1);
  GUI_ClearRect(r.x0, _BoardY0, _BoardX0 - 1, _BoardY0 + (8 * _CellSize));
  GUI_ClearRect(_BoardX0 + (8 * _CellSize) + 1, _BoardY0, r.x1, _BoardY0 + (8 * _CellSize));
  GUI_ClearRect(r.x0, _BoardY0 + (8 * _CellSize) + 1, r.x1, r.y1);
  CellSize = _CellSize - 1;
  rStone = ((CellSize - 3) * AA_CALCFACTOR) >> 1;
  rMove  = ((CellSize - 2) * AA_CALCFACTOR) >> 3;
  if (rStone < AA_CALCFACTOR) {
    rStone = AA_CALCFACTOR;
  }
  if (rMove < (AA_CALCFACTOR >> AA_USE_HIRES)) {
    rMove = (AA_CALCFACTOR >> AA_USE_HIRES);
  }
  for (yPos = _BoardY0, y = 0; y < 8; y++) {
    for (xPos = _BoardX0, x = 0; x < 8; x++) {
      Color = ((x + (y & 1)) & 1) ? (GUI_LIGHTGRAY) : (GUI_WHITE);
      LCD_SetColor(Color);
      GUI_FillRect(xPos + 1, yPos + 1, xPos + CellSize, yPos + CellSize);
      Cell        = _GetStone(&_Board, x, y);
      IsValidMove = (_ShowPossibleMoves) ? _IsValidMove(&_Board, x, y) : 0;
      if (_pPlayerAI[_Board.ActPlayer - 1]) {
        IsValidMove = 0;
      }
      if (Cell || IsValidMove) {
        xCircle = (xPos + 1) * AA_CALCFACTOR + ((CellSize * AA_CALCFACTOR) >> 1);
        yCircle = (yPos + 1) * AA_CALCFACTOR + ((CellSize * AA_CALCFACTOR) >> 1);
        if (Cell) {
          Color = (Cell == 1) ? (GUI_RED) : (GUI_BLUE);
          LCD_SetColor(Color);
          #if (AA_FACTOR > 1)
            GUI_AA_FillCircle(xCircle, yCircle, rStone);
          #else
            GUI_FillCircle(xCircle, yCircle, rStone);
          #endif
        } else {
          LCD_SetColor(GUI_BLACK);
          #if (AA_FACTOR > 1)
            GUI_AA_FillCircle(xCircle, yCircle, rMove);
          #else
            GUI_FillCircle(xCircle, yCircle, rMove);
          #endif
        }
      }
      LCD_SetColor(GRID_COLOR);
      GUI_DrawVLine(xPos, yPos + 1, yPos + CellSize);
      xPos += _CellSize;
    }
    GUI_DrawVLine(xPos, yPos + 1, yPos + CellSize);
    GUI_DrawHLine(yPos, _BoardX0, _BoardX0 + (_CellSize << 3));
    yPos += _CellSize;
  }
  GUI_DrawHLine(yPos, _BoardX0, _BoardX0 + (_CellSize << 3));
}

/**
  * @brief  callback Reversi Win 
  * @param pMsg: pointer to data structure 
  * @retval None
  */
static void _cbReversiWin(WM_MESSAGE* pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint(hWin);
    break;
  case WM_TOUCH:
    _OnTouch(pMsg);
    break;
  case WM_DELETE:
    _hFrame = 0;
    break;    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Game window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  _pPlayerAI[1] = _PlayerAI_SmartGecko;
  _hFrame = FRAMEWIN_CreateEx(xpos, ypos, 640, 439, hWin, WM_CF_SHOW, FRAMEWIN_CF_ACTIVE, 0, NULL, &_cbReversiWin);
 
  FRAMEWIN_SetClientColor(_hFrame, GUI_INVALID_COLOR);
  FRAMEWIN_SetFont       (_hFrame, GUI_FONT_24_ASCII);
  FRAMEWIN_AddCloseButton(_hFrame, FRAMEWIN_BUTTON_RIGHT, 0);
  
  _StartNewGame();
}

/**
  * @}
  */

/**
  * @}
  */
