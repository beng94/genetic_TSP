#include <cstdlib>
#include <vector>

const int MAX_COORDINATE = 500;
const int CITIES = 20;
const int MAX_POPULATION = 50;

class City
{
    private:
        int x, y;
    public:
        City()
        {
            x = std::rand() % MAX_COORDINATE;
            y = std::rand() % MAX_COORDINATE;
        }
};

class Tour
{
    private:
        std::vector<City> tour;
        int fitness;
        int distance;
    public:
        Tour()
        {
            tour.reserve(CITIES);
            for(int i = 0; i < CITIES; i++)
            {
                tour.push_back(City());
            }
        }
};

class Evolution
{
    private:
        std::vector<Tour> population;
    public:
        Evolution()
        {
            population.reserve(MAX_POPULATION);
            for(int i = 0; i<MAX_POPULATION; i++)
            {
                population.push_back(Tour());
            }
        }
        void evolve()
        {

        }
        void crossover();
        void mutate();
        void tournament_selection(); //???
};
