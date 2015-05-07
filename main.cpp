#include <cstdlib>
#include <vector>
#include <iostream>

const int MAX_COORDINATE = 500;
const int CITIES = 20;
const int MAX_POPULATION = 50;
const double MUTATION_RATE = 0.015;
const int TOURNAMENT_SIZE = 5;

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
        Tour(std::vector<City> _tour)
        {
            tour = _tour;
        }
        int get_size() const {return tour.size();}
        City get_city(int id) const {return tour.at(id);}
        std::vector<City> get_tour() const {return tour;}
        int get_fitness() {return fitness;}

        void set_city(City& c, int id) { tour[id] = c;}
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
            for(int i=0 ; i<population.size(); i++)
            {
                Tour father = tournament_selection();
                Tour mother = tournament_selection();
                Tour child = crossover(father, mother);

                population.at(i) = child;
            }

            for(int i= 0; i<population.size(); i++)
            {
                mutate(population.at(i));
            }
        }
        Tour crossover(const Tour& father, const Tour& mother)
        {
            int beg = rand() % father.get_size();
            int end = rand() % father.get_size();

            if(end < beg)
            {
                int tmp = end;
                end = beg;
                beg = end;
            }

            std::vector<City> new_tour;
            new_tour.reserve(father.get_size());

            for(int i = 0; i<beg; i++)
            {
                new_tour.at(i) = mother.get_tour().at(i);
            }

            new_tour.insert(new_tour.begin() + beg,
                            father.get_tour().begin() + beg,
                            father.get_tour().begin() + end);

            for(int i = end; i < father.get_size(); i++)
            {
                new_tour.at(i) = mother.get_tour().at(i);
            }

            return Tour(new_tour);
        }
        void mutate(Tour& tour)
        {
            for(int i= 0; i<tour.get_size(); i++)
            {
                if((1 / rand()) < MUTATION_RATE)
                {
                    //swap cities
                    int id = rand() % tour.get_size();
                    City a = tour.get_city(i);
                    City b = tour.get_city(id);

                    tour.set_city(b, i);
                    tour.set_city(a, id);
                }
            }
        }
        Tour tournament_selection()
        {
            std::vector<Tour> tournament;
            tournament.reserve(TOURNAMENT_SIZE);

            for(int i= 0; i<TOURNAMENT_SIZE; i++)
            {
                int id = rand() % population.size();
                tournament.at(i) = population.at(id);
            }

            //get fittest
            Tour max_fit = tournament.at(0);
            int fitness = max_fit.get_fitness();

            for(int i=1; i<TOURNAMENT_SIZE; i++)
            {
                if(tournament.at(i).get_fitness() > fitness)
                {
                    max_fit = tournament.at(i);
                    fitness = max_fit.get_fitness();
                }
            }

            return max_fit;
        }
};

int main()
{
    std::cout << (double)1 / rand() << std::endl;

    return 0;
}
