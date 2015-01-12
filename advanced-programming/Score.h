/*	Shoot a Turret - 100 pts
	Shoot a Fireball - 200 pts
	Shoot the exhaust port - 1000 pts
	Complete trench by firing only 1 shot, and hitting the exhaust
	port - 10,000
*/
#ifndef SCORE_H
#define SCORE_H

class Score 
{
private:
	int score, lives, baseLives;

public:
	// constructor
	Score(int _baseLives = 3)
	{
		baseLives = _baseLives;
	}
	// called when points are scored
	void Score::PointsScored(int bonus) { score += bonus; }
	// called to return score
	int Score::GetScore() { return score; }
	// add/subtract lives
	void Score::AddLives(int lifeChange) 
	{
		if (lives < 99) { lives += lifeChange; }
	}
	// check current lives
	int Score::CheckLives() { return lives; }
	// reset lives and score to new game values
	void Score::Reset() 
	{
		lives = baseLives;
		score = 0;
	}
		
	// destructor
	~Score() {}
};
#endif