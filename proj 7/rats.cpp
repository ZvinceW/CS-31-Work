





// rats.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;            // max number of rows in the arena
const int MAXCOLS = 20;            // max number of columns in the arena
const int MAXRATS = 100;           // max number of rats allowed

const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
const int NUMDIRS = 4;

const int EMPTY = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
			  // type name, since it's mentioned in the Rat declaration.

class Rat
{
public:
	// Constructor
	Rat(Arena* ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	bool isDead() const;

	// Mutators
	void move();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	// [DONE]TODO: You'll probably find that a rat object needs additional
	// data members to support your implementation of the behavior affected
	// by poison pellets.
	bool m_deadRat;      //[DONE]TODO: set to false in constructor pls 
	int m_poisonTimes;   //[DONE]TODO: set to 0 in constructor pls
	int m_poisonTurns;   //[DONE]TODO: set to 0 in constructor pls 
	int getPoisoned(int& turns);
};

class Player
{
public:
	// Constructor
	Player(Arena* ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	bool isDead() const;

	// Mutators
	string dropPoisonPellet();
	string move(int dir);
	void   setDead();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	bool   m_dead;
};

class Arena
{
public:
	// Constructor/destructor
	Arena(int nRows, int nCols);
	~Arena();

	// Accessors
	int     rows() const;
	int     cols() const;
	Player* player() const;
	int     ratCount() const;
	int     getCellStatus(int r, int c) const;
	int     numberOfRatsAt(int r, int c) const;
	void    display(string msg) const;

	// Mutators
	void setCellStatus(int r, int c, int status);
	bool addRat(int r, int c);
	bool addPlayer(int r, int c);
	void moveRats();

private:
	int     m_grid[MAXROWS][MAXCOLS];
	int     m_rows;
	int     m_cols;
	Player* m_player;
	Rat*    m_rats[MAXRATS];
	int     m_nRats;
	int     m_turns;

	// Helper functions
	void checkPos(int r, int c) const;
	bool isPosInBounds(int r, int c) const;
};

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nRats);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;

	// Helper functions
	string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int min, int max);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Rat implementation
///////////////////////////////////////////////////////////////////////////

Rat::Rat(Arena* ap, int r, int c)
{
	if (ap == nullptr)
	{
		cout << "***** A rat must be created in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "***** Rat created with invalid coordinates (" << r << ","
			<< c << ")!" << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	m_poisonTimes = 0;   //refer to line 64
	m_poisonTurns = 0;
	m_deadRat = false;
}

int Rat::row() const
{
	return m_row;
}

int Rat::col() const
{
	// [DONE]TODO: TRIVIAL:  Return what column the Rat is at
	// Delete the following line and replace it with the correct code.
	return m_col;
}

bool Rat::isDead() const
{
	// [DONE]TODO: Return whether the Rat is dead
	// Delete the following line and replace it with the correct code.
	return m_deadRat;
}

void Rat::move()
{
	// [DONE?]TODO:
	//   Return without moving if the rat has eaten one poison pellet (so
	//   is supposed to move only every other turn) and this is a turn it
	//   does not move.

	//   Otherwise, attempt to move in a random direction; if it can't
	//   move, don't move.  If it lands on a poison pellet, eat the pellet,
	//   so it is no longer on that grid point.

	if (m_poisonTimes == 1) {
		int moveOrNot = getPoisoned(m_poisonTurns);
		if (moveOrNot == 0) {
			return;
		}                 
	}
	int dir = randInt(NORTH, WEST); //generate a random direction
	//attempt to move in the generated direction
	if (attemptMove(*m_arena, dir, m_row, m_col)) {
		if (m_arena->player()->row() == m_row && m_arena->player()->col() == m_col) {
			m_arena->player()->setDead(); //mouse walked into the player and player died ( [DONE]TODO: should i display a msg)
		}
		// [DONE]TODO: maybe make this an else if
		if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON) {
			m_poisonTimes++;
			//[DONE]TODO: delete the poison pellet
			m_arena->setCellStatus(m_row, m_col, EMPTY); //this should replace poison cell with empty cell
		}
		if (m_poisonTimes == 2) {
			m_deadRat = true;      //rat is dead if 2 pellets are eaten
		}
	}
	return;
}

int Rat::getPoisoned(int& turns) {   //determine if the rat should move that turn when poisoned once already
	if (turns % 2 == 0) {
		turns++;
		return 0;  //dont move
	}
	else {
		turns++;
		return 1;  //move
	}
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
	if (ap == nullptr)
	{
		cout << "***** The player must be created in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "**** Player created with invalid coordinates (" << r
			<< "," << c << ")!" << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	m_dead = false;
}

int Player::row() const
{
	// [DONE]TODO: TRIVIAL:  Return what row the Player is at
	// Delete the following line and replace it with the correct code.
	return m_row;
}

int Player::col() const
{
	// [DONE]TODO: TRIVIAL:  Return what column the Player is at
	// Delete the following line and replace it with the correct code.
	return m_col;
}

string Player::dropPoisonPellet()
{
	if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
		return "There's already a poison pellet at this spot.";
	m_arena->setCellStatus(m_row, m_col, HAS_POISON);
	return "A poison pellet has been dropped.";
}

string Player::move(int dir)
{
	// [DONE]TODO:  Attempt to move the player one step in the indicated
	//        direction.  If this fails,
	//        return "Player couldn't move; player stands."
	//        A player who moves onto a rat dies, and this
	//        returns "Player walked into a rat and died."
	//        Otherwise, return one of "Player moved north.",
	//        "Player moved east.", "Player moved south.", or
	//        "Player moved west."
	
	if (attemptMove(*m_arena, dir, m_row, m_col)) {
		if (m_arena->numberOfRatsAt(m_row, m_col) > 0) {
			m_dead = true;
			return "Player walked into a rat and died.";    
		}
		else {
			if (dir == NORTH)
				return "Player moved north.";
			else if (dir == SOUTH)
				return "Player moved south.";
			else if (dir == EAST)
				return "Player moved east";
			else if (dir == WEST)
				return "Player moved west";
			else
				return "this is impossible to reach";
		}
	}
	else {
		return "Player couldn't move; player stands.";
	}
}

bool Player::isDead() const
{
	// [DONE]TODO: Return whether the Player is dead
	// Delete the following line and replace it with the correct code.
	return m_dead;
}

void Player::setDead()
{
	m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
	if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
	{
		cout << "***** Arena created with invalid size " << nRows << " by "
			<< nCols << "!" << endl;
		exit(1);
	}
	m_rows = nRows;
	m_cols = nCols;
	m_player = nullptr;
	m_nRats = 0;
	m_turns = 0;
	for (int r = 1; r <= m_rows; r++)
		for (int c = 1; c <= m_cols; c++)
			setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
	// [DONE?]TODO:  Deallocate the player and all remaining dynamically allocated rats
	delete m_player;
	for (int i = 0; i < ratCount(); i++) {
		delete m_rats[i];
	}
}

int Arena::rows() const
{
	// [DONE]TODO: TRIVIAL:  Return the number of rows in the arena
	// Delete the following line and replace it with the correct code.
	return m_rows;
}

int Arena::cols() const
{
	// [DONE]TODO: TRIVIAL:  Return the number of columns in the arena
	// Delete the following line and replace it with the correct code.
	return m_cols;
}

Player* Arena::player() const
{
	return m_player;
}

int Arena::ratCount() const
{
	// [DONE]TODO: TRIVIAL:  Return the number of rats in the arena
	// Delete the following line and replace it with the correct code.
	return m_nRats;
}

int Arena::getCellStatus(int r, int c) const
{
	checkPos(r, c);
	return m_grid[r - 1][c - 1];
}

int Arena::numberOfRatsAt(int r, int c) const
{
	// [DONE]TODO:  Return the number of rats at row r, column c
	// Delete the following line and replace it with the correct code.
	int ratNum = 0;
	for (int i = 0; i < ratCount(); i++) {
		if (m_rats[i]->row() == r && m_rats[i]->col() == c) {
			ratNum++;
		}
	}
	return ratNum;
}

void Arena::display(string msg) const
{
	char displayGrid[MAXROWS][MAXCOLS];
	int r, c;

	// Fill displayGrid with dots (empty) and stars (poison pellets)
	for (r = 1; r <= rows(); r++)
		for (c = 1; c <= cols(); c++)
			displayGrid[r - 1][c - 1] = (getCellStatus(r, c) == EMPTY ? '.' : '*');

	// Indicate each rat's position
	// [DONE?]TODO:  If one rat is at some grid point, set the displayGrid char
	//        to 'R'.  If it's 2 though 8, set it to '2' through '8'.
	//        For 9 or more, set it to '9'.
	for (int i = 0; i < ratCount(); i++) {
		if (m_rats[i] != nullptr) {
			if (displayGrid[m_rats[i]->row() - 1][m_rats[i]->col() - 1] == '.') {
				displayGrid[m_rats[i]->row() - 1][m_rats[i]->col() - 1] = 'R';
			}
			else if (displayGrid[m_rats[i]->row() - 1][m_rats[i]->col() - 1] == 'R') {
				displayGrid[m_rats[i]->row() - 1][m_rats[i]->col() - 1] = '2';
			}
			else if (isdigit(displayGrid[m_rats[i]->row() - 1][m_rats[i]->col() - 1]) && displayGrid[m_rats[i]->row() - 1][m_rats[i]->col() - 1] >= '2' && displayGrid[m_rats[i]->row() - 1][m_rats[i]->col() - 1] <= '8') {
				displayGrid[m_rats[i]->row() - 1][m_rats[i]->col() - 1]++;
			}
			else {
				displayGrid[m_rats[i]->row() - 1][m_rats[i]->col() - 1] = '9';
			}
		}
	}

	// Indicate player's position
	if (m_player != nullptr)
		displayGrid[m_player->row() - 1][m_player->col() - 1] = (m_player->isDead() ? 'X' : '@');

	// Draw the grid
	clearScreen();
	for (r = 1; r <= rows(); r++)
	{
		for (c = 1; c <= cols(); c++)
			cout << displayGrid[r - 1][c - 1];
		cout << endl;
	}
	cout << endl;

	// Write message, rat, and player info
	if (msg != "")
		cout << msg << endl;
	cout << "There are " << ratCount() << " rats remaining." << endl;
	if (m_player == nullptr)
		cout << "There is no player!" << endl;
	else if (m_player->isDead())
		cout << "The player is dead." << endl;
	cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
	checkPos(r, c);
	m_grid[r - 1][c - 1] = status;
}

bool Arena::addRat(int r, int c)
{
	if (!isPosInBounds(r, c))
		return false;

	// Don't add a rat on a spot with a poison pellet
	if (getCellStatus(r, c) != EMPTY)
		return false;

	// Don't add a rat on a spot with a player
	if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
		return false;

	// If there are MAXRATS existing rats, return false.  Otherwise,
	// dynamically allocate a new rat at coordinates (r,c).  Save the
	// pointer to the newly allocated rat and return true.

	// [DONE]TODO:  Implement this.
	if (ratCount() == MAXRATS) {
		return false;
	}
	else {
		m_rats[ratCount()] = new Rat(this, r, c);
		m_nRats++;   //CHECKPOINT
		return true;
	}
}

bool Arena::addPlayer(int r, int c)
{
	if (!isPosInBounds(r, c))
		return false;

	// Don't add a player if one already exists
	if (m_player != nullptr)
		return false;

	// Don't add a player on a spot with a rat
	if (numberOfRatsAt(r, c) > 0)
		return false;

	m_player = new Player(this, r, c);
	return true;
}

void Arena::moveRats()
{
	// Move all rats
	// [DONE]TODO:  Move each rat.  Mark the player as dead if necessary.   TODO2: check if the Rat move() member function marks player dead
	//        Deallocate any dead dynamically allocated rat.
	for (int i = 0; i < m_nRats; i++) {
		if (m_rats[i] != nullptr) {
			m_rats[i]->move();
		}                    // move all rats in the list of rats
	}
	for (int i = 0; i < m_nRats; i++) {
		if (m_rats[i] != nullptr && m_rats[i]->isDead()) {
			delete m_rats[i];                   //find dead rats from the list of rats and delete them
			m_rats[i] = m_rats[m_nRats - 1];
			m_nRats--;
		}
	}

	// Another turn has been taken
	m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
	return (r >= 1 && r <= m_rows  &&  c >= 1 && c <= m_cols);
}

void Arena::checkPos(int r, int c) const
{
	if (r < 1 || r > m_rows || c < 1 || c > m_cols)
	{
		cout << "***** " << "Invalid arena position (" << r << ","
			<< c << ")" << endl;
		exit(1);
	}
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRats)
{
	if (nRats < 0)
	{
		cout << "***** Cannot create Game with negative number of rats!" << endl;
		exit(1);
	}
	if (nRats > MAXRATS)
	{
		cout << "***** Trying to create Game with " << nRats
			<< " rats; only " << MAXRATS << " are allowed!" << endl;
		exit(1);
	}
	if (rows == 1 && cols == 1 && nRats > 0)
	{
		cout << "***** Cannot create Game with nowhere to place the rats!" << endl;
		exit(1);
	}

	// Create arena
	m_arena = new Arena(rows, cols);

	// Add player
	int rPlayer;
	int cPlayer;
	do
	{
		rPlayer = randInt(1, rows);
		cPlayer = randInt(1, cols);
	} while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
	m_arena->addPlayer(rPlayer, cPlayer);

	// Populate with rats
	while (nRats > 0)
	{
		int r = randInt(1, rows);
		int c = randInt(1, cols);
		if (r == rPlayer && c == cPlayer)
			continue;
		m_arena->addRat(r, c);
		nRats--;
	}
}

Game::~Game()
{
	delete m_arena;
}

string Game::takePlayerTurn()
{
	for (;;)
	{
		cout << "Your move (n/e/s/w/x or nothing): ";
		string playerMove;
		getline(cin, playerMove);

		Player* player = m_arena->player();
		int dir;

		if (playerMove.size() == 0)
		{
			if (recommendMove(*m_arena, player->row(), player->col(), dir)) {
				cout << dir << endl;
				return player->move(dir);
			}
			else
				return player->dropPoisonPellet();
		}
		else if (playerMove.size() == 1)
		{
			if (tolower(playerMove[0]) == 'x')
				return player->dropPoisonPellet();
			else if (decodeDirection(playerMove[0], dir))
				return player->move(dir);
		}
		cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
	}
}

void Game::play()
{
	m_arena->display("");
	while (!m_arena->player()->isDead() && m_arena->ratCount() > 0)
	{
		string msg = takePlayerTurn();
		Player* player = m_arena->player();
		if (player->isDead())
		{
			cout << msg << endl;
			break;
		}
		m_arena->moveRats();
		m_arena->display(msg);
	}
	if (m_arena->player()->isDead())
		cout << "You lose." << endl;
	else
		cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

// Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
	if (max < min)
		swap(max, min);
	static random_device rd;
	static mt19937 generator(rd());
	uniform_int_distribution<> distro(min, max);
	return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
	switch (tolower(ch))
	{
	default:  return false;
	case 'n': dir = NORTH; break;
	case 'e': dir = EAST;  break;
	case 's': dir = SOUTH; break;
	case 'w': dir = WEST;  break;
	}
	return true;
}

// Return false without changing anything if moving one step from (r,c)
// in the indicated direction would run off the edge of the arena.
// Otherwise, update r and c to the position resulting from the move and
// return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
	// [DONE]TODO:  Implement this function
	if (dir == NORTH) {
		if (r - 1 < 1) {
			return false;
		}
		else {
			r--;
			return true;
		}
	}
	else if (dir == SOUTH) {
		if (r + 1 > a.rows()) {
			return false;
		}
		else {
			r++;
			return true;
		}
	}
	else if (dir == EAST) {
		if (c + 1 > a.cols()) {
			return false;
		}
		else {
			c++;
			return true;
		}
	}
	else if (dir == WEST) {
		if (c - 1 < 1) {
			return false;
		}
		else { 
			c--; 
			return true;
		}
	}
	return false;  //if the direction is not N, S, E, or W
}

// Recommend a move for a player at (r,c):  A false return means the
// recommendation is that the player should drop a poison pellet and not
// move; otherwise, this function sets bestDir to the recommended
// direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
	// [DONE?]TODO:  Implement this function
	// Delete the following line and replace it with your code.
	
	//safe rating of all 8 directions
	int nSafe = 0;
	int n1Safe = 0; //north + 1
	int sSafe = 0;
	int s1Safe = 0; //south + 1
	int eSafe = 0;
	int e1Safe = 0; //east + 1
	int wSafe = 0;
	int w1Safe = 0; //west + 1
	int neSafe = 0;
	int nwSafe = 0;
	int seSafe = 0;
	int swSafe = 0;

	//total rat # in the 4 general directions AND total rats in middle
	int nTotal = MAXRATS + 1;
	int sTotal = MAXRATS + 1;
	int eTotal = MAXRATS + 1;
	int wTotal = MAXRATS + 1;
	int midTotal = MAXRATS + 1;

	//check nSafe
	if (r - 1 < 1) {
		nSafe = MAXRATS + 1;
	}
	else if (a.numberOfRatsAt(r - 1, c) >= 0) {
		nSafe = a.numberOfRatsAt(r - 1, c);
	}
	//check n1safe
	if (r - 2 < 1) {
		n1Safe = 0;
	}
	else if (a.numberOfRatsAt(r - 2, c) >= 0) {
		n1Safe = a.numberOfRatsAt(r - 2, c);
	}
	//check sSafe
	if (r + 1 > a.rows()) {
		sSafe = MAXRATS + 1;
	}
	else if (a.numberOfRatsAt(r + 1, c) >= 0) {
		sSafe = a.numberOfRatsAt(r + 1, c);
	}
	//check s1Safe
	if (r + 2 > a.rows()) {
		s1Safe = 0;
	}
	else if (a.numberOfRatsAt(r + 2, c) >= 0) {
		s1Safe = a.numberOfRatsAt(r + 2, c);
	}
	//check eSafe
	if (c + 1 > a.cols()) {
		eSafe = MAXRATS + 1;
	}
	else if (a.numberOfRatsAt(r, c + 1) >= 0) {
		eSafe = a.numberOfRatsAt(r, c + 1);
	}
	//check e1Safe
	if (c + 2 > a.cols()) {
		e1Safe = 0;
	}
	else if (a.numberOfRatsAt(r, c + 2) >= 0) {
		e1Safe = a.numberOfRatsAt(r, c + 2);
	}
	//check wSafe
	if (c - 1 < 1) {
		wSafe = MAXRATS + 1;
	}
	else if (a.numberOfRatsAt(r, c - 1) >= 0) {
		wSafe = a.numberOfRatsAt(r, c - 1);
	}
	//check w1Safe
	if (c - 2 < 1) {
		w1Safe = 0;
	}
	else if (a.numberOfRatsAt(r, c - 2) >= 0) {
		w1Safe = a.numberOfRatsAt(r, c - 2);
	}
	//check neSafe
	if (r - 1 < 1 || c + 1 > a.cols()) {
		neSafe = 0;
	}
	else if (a.numberOfRatsAt(r - 1, c + 1) >= 0) {
		neSafe = a.numberOfRatsAt(r - 1, c + 1);
	}
	//check nwSafe
	if (r - 1 < 1 || c - 1 < 1) {
		nwSafe = 0;
	}
	else if (a.numberOfRatsAt(r - 1, c - 1) >= 0) {
		nwSafe = a.numberOfRatsAt(r - 1, c - 1);
	}
	//check seSafe
	if (r + 1 > a.rows() || c + 1 > a.cols()) {
		seSafe = 0;
	}
	else if (a.numberOfRatsAt(r + 1, c + 1) >= 0) {
		seSafe = a.numberOfRatsAt(r + 1, c + 1);
	}
	//check swSafe
	if (r + 1 > a.rows() || c - 1 < 1) {
		swSafe = 0;
	}
	else if (a.numberOfRatsAt(r + 1, c - 1) >= 0) {
		swSafe = a.numberOfRatsAt(r + 1, c - 1);
	}

	//stay put if surrounded by empty
	if ((nSafe == 0) && (sSafe == 0) && (eSafe == 0) && (wSafe == 0)) {
		return false;
	}

	//stay put if surrounded by rats in 4 directions
	if ((nSafe == MAXRATS) + 1 && (sSafe == MAXRATS + 1) && (eSafe == MAXRATS + 1) && (wSafe == MAXRATS + 1)) {
		return false;
	}

	//calculate nTotal
	if ((nSafe == 0)) {
		nTotal = 0;
		if (n1Safe >= 0) {
			nTotal += n1Safe;
		}
		if (nwSafe >= 0) {
			nTotal += nwSafe;
		}
		if (neSafe >= 0) {
			nTotal += neSafe;
		}
	}

	//calculate sTotal
	if ((sSafe == 0)) {
		sTotal = 0;
		if (s1Safe >= 0) {
			sTotal += s1Safe;
		}
		if (swSafe >= 0) {
			sTotal += swSafe;
		}
		if (seSafe >= 0) {
			sTotal += seSafe;
		}
	}

	//calculate eTotal
	if ((eSafe == 0)) {
		eTotal = 0;
		if (e1Safe >= 0) {
			eTotal += e1Safe;
		}
		if (neSafe >= 0) {
			eTotal += neSafe;
		}
		if (seSafe >= 0) {
			eTotal += seSafe;
		}
	}

	//calculate wTotal
	if ((wSafe == 0)) {
		wTotal = 0;
		if (w1Safe >= 0) {
			wTotal += w1Safe;
		}
		if (nwSafe >= 0) {
			wTotal += nwSafe;
		}
		if (swSafe >= 0) {
			wTotal += swSafe;
		}
	}

	//calculate midTotal
	if (sSafe >= 0 && sSafe < MAXRATS + 1) {
		midTotal += sSafe;
	}
	if (nSafe >= 0 && nSafe < MAXRATS + 1) {
		midTotal += nSafe;
	}
	if (eSafe >= 0 && nSafe < MAXRATS + 1) {
		midTotal += eSafe;
	}
	if (wSafe >= 0 && wSafe < MAXRATS + 1) {
		midTotal += wSafe;
	}

	//figure out the best direction (n, s, e, w, mid)
	if (nTotal <= sTotal && nTotal <= eTotal && nTotal <= wTotal && nTotal <= midTotal) {
		bestDir = NORTH;
		return true;
	}
	if (sTotal <= nTotal && sTotal <= eTotal && sTotal <= wTotal && sTotal <= midTotal) {
		bestDir = SOUTH;
		return true;
	}
	if (eTotal <= sTotal && eTotal <= nTotal && eTotal <= wTotal && eTotal <= midTotal) {
		bestDir = EAST;
		return true;
	}
	if (wTotal <= sTotal && wTotal <= eTotal && wTotal <= nTotal && wTotal <= midTotal) {
		bestDir = WEST;
		return true;
	}

	return false;

	
				   // Your replacement implementation should do something intelligent.
				   // You don't have to be any smarter than the following, although
				   // you can if you want to be:  If staying put runs the risk of a
				   // rat possibly moving onto the player's location when the rats
				   // move, yet moving in a particular direction puts the player in a
				   // position that is safe when the rats move, then the chosen
				   // action is to move to a safer location.  Similarly, if staying put
				   // is safe, but moving in certain directions puts the player in danger,
				   // then the chosen action should not be to move in one of the
				   // dangerous directions; instead, the player should stay put or move to
				   // another safe position.  In general, a position that may be moved to
				   // by many rats is more dangerous than one that may be moved to by
				   // few.
				   //
				   // Unless you want to, you do not have to take into account that a
				   // rat might be poisoned and thus sometimes less dangerous than one
				   // that is not.  That requires a more sophisticated analysis that
				   // we're not asking you to do.
}


//CODE BELOW IS FOR BASIC TESTING
/*
#include <type_traits>
#include <cassert>

#define CHECKTYPE(c, f, r, a)  \
	static_assert(std::is_same<decltype(&c::f), r (c::*)a>::value, \
	   "FAILED: You changed the type of " #c "::" #f);  \
	[[gnu::unused]] r (c::* xxx##c##_##f) a = &c::f

void thisFunctionWillNeverBeCalled()
{
	// If the student deleted or changed the interfaces to the public
	// functions, this won't compile.  (This uses magic beyond the scope
	// of CS 31.)

	Rat r(static_cast<Arena*>(0), 1, 1);
	CHECKTYPE(Rat, row, int, () const);
	CHECKTYPE(Rat, col, int, () const);
	CHECKTYPE(Rat, isDead, bool, () const);
	CHECKTYPE(Rat, move, void, ());

	Player p(static_cast<Arena*>(0), 1, 1);
	CHECKTYPE(Player, row, int, () const);
	CHECKTYPE(Player, col, int, () const);
	CHECKTYPE(Player, isDead, bool, () const);
	CHECKTYPE(Player, dropPoisonPellet, string, ());
	CHECKTYPE(Player, move, string, (int));
	CHECKTYPE(Player, setDead, void, ());

	Arena a(1, 1);
	CHECKTYPE(Arena, rows, int, () const);
	CHECKTYPE(Arena, cols, int, () const);
	CHECKTYPE(Arena, player, Player*, () const);
	CHECKTYPE(Arena, ratCount, int, () const);
	CHECKTYPE(Arena, getCellStatus, int, (int, int) const);
	CHECKTYPE(Arena, numberOfRatsAt, int, (int, int) const);
	CHECKTYPE(Arena, display, void, (string) const);
	CHECKTYPE(Arena, setCellStatus, void, (int, int, int));
	CHECKTYPE(Arena, addRat, bool, (int, int));
	CHECKTYPE(Arena, addPlayer, bool, (int, int));
	CHECKTYPE(Arena, moveRats, void, ());

	Game g(1, 1, 1);
	CHECKTYPE(Game, play, void, ());
}

void findTheRat(const Arena& a, int& r, int& c)
{
	if (a.numberOfRatsAt(r - 1, c) == 1) r--;
	else if (a.numberOfRatsAt(r + 1, c) == 1) r++;
	else if (a.numberOfRatsAt(r, c - 1) == 1) c--;
	else if (a.numberOfRatsAt(r, c + 1) == 1) c++;
	else assert(false);
}

void doBasicTests()
{
	{
		Arena a(10, 20);
		a.addPlayer(2, 5);
		Player* pp = a.player();
		assert(pp->row() == 2 && pp->col() == 5 && !pp->isDead());
		assert(pp->move(NORTH) == "Player moved north.");
		assert(pp->row() == 1 && pp->col() == 5 && !pp->isDead());
		assert(pp->move(NORTH) == "Player couldn't move; player stands.");
		assert(pp->row() == 1 && pp->col() == 5 && !pp->isDead());
		pp->setDead();
		assert(pp->row() == 1 && pp->col() == 5 && pp->isDead());
	}
	{
		Arena a(10, 20);
		int r = 4;
		int c = 4;
		a.setCellStatus(r - 1, c, HAS_POISON);
		a.setCellStatus(r + 1, c, HAS_POISON);
		a.setCellStatus(r, c - 1, HAS_POISON);
		a.setCellStatus(r, c + 1, HAS_POISON);
		a.addRat(r, c);
		a.addPlayer(8, 18);
		assert(a.ratCount() == 1 && a.numberOfRatsAt(r, c) == 1);
		a.moveRats();
		assert(a.ratCount() == 1 && a.numberOfRatsAt(r, c) == 0);
		findTheRat(a, r, c);
		assert(a.getCellStatus(r, c) != HAS_POISON);
		a.moveRats();
		assert(a.ratCount() == 1 && a.numberOfRatsAt(r, c) == 1);
		a.moveRats();
		assert(a.ratCount() == 1 && a.numberOfRatsAt(r, c) == 0);
		findTheRat(a, r, c);
		a.moveRats();
		assert(a.ratCount() == 1 && a.numberOfRatsAt(r, c) == 1);
		a.setCellStatus(r - 1, c, HAS_POISON);
		a.setCellStatus(r + 1, c, HAS_POISON);
		a.setCellStatus(r, c - 1, HAS_POISON);
		a.setCellStatus(r, c + 1, HAS_POISON);
		a.moveRats();
		assert(a.ratCount() == 0 && a.numberOfRatsAt(r, c) == 0);
		assert(a.numberOfRatsAt(r - 1, c) == 0);
		assert(a.numberOfRatsAt(r + 1, c) == 0);
		assert(a.numberOfRatsAt(r, c - 1) == 0);
		assert(a.numberOfRatsAt(r, c + 1) == 0);

		for (int k = 0; k < MAXRATS / 4; k++)
		{
			a.addRat(7, 18);
			a.addRat(9, 18);
			a.addRat(8, 17);
			a.addRat(8, 19);
		}
		assert(!a.player()->isDead());
		a.moveRats();
		assert(a.player()->isDead());
	}
	cout << "Passed all basic tests" << endl;
}
*/

//CODE ABOVE IS FOR BASIC TESTING


///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
	//doBasicTests(); // Remove this line after completing test.
	//return 0;       // Remove this line after completing 

	// Create a game
	// Use this instead to create a mini-game:   
	Game g(5, 5, 3);
	//Game g(10, 12, 40);

	// Play the game
	g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD upperLeft = { 0, 0 };
	DWORD dwCharsWritten;
	FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
		&dwCharsWritten);
	SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
	static const char* term = getenv("TERM");
	if (term == nullptr || strcmp(term, "dumb") == 0)
		cout << endl;
	else
	{
		static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
		cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
	}
}

#endif