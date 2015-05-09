#include <cstdlib>
#include <vector>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <algorithm>

const int MAX_COORDINATE = 500;
const int CITIES = 20;
const int MAX_POPULATION = 50;
const double MUTATION_RATE = 0.015;
const int TOURNAMENT_SIZE = 5;
const int GENERATIONS = 1000;

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
        double distance(const City& c)
        {
            int _x = x - c.get_x();
            int _y = y - c.get_y();

            return sqrt(_x*_x + _y*_y);
        }

        int get_x() const {return x;}
        int get_y() const {return y;}

        bool operator== (const City& c)
        {
            return (x == c.get_x() && y == c.get_y());
        }
        bool operator != (const City& c)
        {
            return !(*this == c);
        }
        friend std::ostream& operator<< (std::ostream& os, const City& c);
};

std::ostream& operator<< (std::ostream& os, const City& c)
{
    os << c.x << " " << c.y;
}

class Tour
{
    private:
        std::vector<City> tour;
        int fitness;
        double distance;
    public:
        Tour()
        {
            //tour.reserve(CITIES);
            for(int i = 0; i < CITIES; i++)
            {
                tour.push_back(City());
            }
            calc_fitness();
        }
        Tour(std::vector<City> _tour)
        {
            tour = _tour;
            calc_fitness();
        }

        void operator= (Tour& _tour)
        {
            tour = _tour.get_tour();
            fitness = _tour.get_fitness();
            distance = _tour.get_dist();
        }

        bool operator== (Tour& _tour)
        {
            for(int i = 0; i < tour.size(); i++)
            {
                if(_tour.get_city(i) != tour.at(i)) return false;
            }
            return true;
        }

        void calc_fitness()
        {
            for(int i = 1; i< tour.size(); i++)
            {
                distance += tour.at(i).distance(tour.at(i-1));
            }
            fitness = (int)distance;
        }

        void print (const char* file_name)
        {
            std::ofstream file;
            file.open(file_name);

            for(int i = 0; i<tour.size(); i++)
            {
                std::cout << '(' << i << ')' << " " << tour.at(i) << " ";
                file << tour.at(i) << std::endl;
            }

            file.close();
        }

        void shuffle ()
        {
            std::random_shuffle(tour.begin(), tour.end());
        }
        int get_size() const {return tour.size();}
        City get_city(int id) const {return tour.at(id);}
        std::vector<City> get_tour() const {return tour;}
        int get_fitness() {return fitness;}
        double get_dist() { return distance; }

        void set_city(City& c, int id)
        {
            tour.at(id) = c;
            calc_fitness();
        }
};

class Evolution
{
    private:
        std::vector<Tour> population;
    public:
        Evolution()
        {
            Tour new_tour;
            //population.reserve(MAX_POPULATION);
            for(int i = 0; i<MAX_POPULATION; i++)
            {
                new_tour.shuffle();
                population.push_back(new_tour);
            }
        }
        void evolve(FILE* pipe)
        {
            for(int i=0 ; i<population.size(); i++)
            {
                population.at(i).print("tour.txt");
                fprintf(pipe, "plot 'points.txt' using 1:2, 'tour.txt' using 1:2 with lines\n");
                fflush(pipe);
                getchar();

                Tour father = tournament_selection();
                father.print("tour.txt");
                std::cout << "father: \n";
                fprintf(pipe, "plot 'points.txt' using 1:2, 'tour.txt' using 1:2 with lines\n");
                fflush(pipe);
                getchar();

                Tour mother = tournament_selection();

                std::cout << "Tournament selection 159\n";
                population.at(i).print("tour.txt");
                fprintf(pipe, "plot 'points.txt' using 1:2, 'tour.txt' using 1:2 with lines\n");
                fflush(pipe);
                getchar();

                Tour child = crossover(father, mother);
                if(child == mother || child == father) continue;


                std::cout << "New child crated 170\n";
                population.at(i).print("tour.txt");
                fprintf(pipe, "plot 'points.txt' using 1:2, 'tour.txt' using 1:2 with lines\n");
                fflush(pipe);
                getchar();

                population.at(i) = child;

                std::cout << "New child added 177\n";
                population.at(i).print("tour.txt");
                fprintf(pipe, "plot 'points.txt' using 1:2, 'tour.txt' using 1:2 with lines\n");
                fflush(pipe);
                getchar();



            }

            for(int i= 0; i<population.size(); i++)
            {
                //mutate(population.at(i));
            }
        }
        Tour crossover(const Tour& father, const Tour& mother) const
        {
            int beg = rand() % father.get_size();
            int end = rand() % father.get_size();

            std::cout << "beg: " << beg << " end: " << end << std::endl;

            if(end < beg)
            {
                int tmp = end;
                end = beg;
                beg = end;
            }
            if(end == beg)
            {
                if(beg > 0) --beg;
                else if(end < father.get_size()) ++end;
                else --end;
            }

            std::vector<City> new_tour;
            //new_tour.reserve(father.get_size());

            for(int i = 0; i<beg; i++)
                new_tour.push_back(mother.get_tour().at(i));

            for(int i = beg; i < end; i++)
                new_tour.push_back(father.get_tour().at(i));

            for(int i = end; i < father.get_size(); i++)
                new_tour.push_back(mother.get_tour().at(i));

            std::cout << "new_tour:\n" << std::endl;
            for(int i= 0; i<new_tour.size(); i++)
            {
                std::cout << new_tour.at(i) << " ";
            }

            Tour t (new_tour);
            std::cout << "copied tour\n" << std::endl;
            for(int i=0; i< t.get_size(); i++)
            {
                std::cout << t.get_tour().at(i) << " ";
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
                    //TODO: use std::swap if possible
                    int id = rand() % tour.get_size();
                    if(id == tour.get_size() - 1) continue;

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
                tournament.push_back(population.at(id));
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

        Tour get_fittest ()
        {
            Tour max = population.at(0);
            for(int i= 1; i<population.size(); i++)
            {
                if(population.at(i).get_fitness() > max.get_fitness())
                {
                    max = population.at(i);
                }
            }

            return max;
        }

        Tour get_tour(int id) {return population.at(id);}
};

int main()
{
    FILE *pipe = popen("gnuplot -persist", "w");

    Evolution evolution;
    evolution.get_tour(0).print("points.txt");
    for(int i = 0; i<GENERATIONS; i++)
    {
        evolution.evolve(pipe);
        evolution.get_tour(0).print("tour.txt");
        fprintf(pipe, "plot 'points.txt' using 1:2, 'tour.txt' using 1:2 with lines\n");
        fflush(pipe);
        getchar();
    }

    fclose(pipe);

    return 0;
}
