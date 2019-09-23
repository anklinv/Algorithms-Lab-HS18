#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

// choose exact integral type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;

typedef K::Point_2 Point;

using namespace std;

struct Asteroid {
	Asteroid(int _x, int _y, int _d) : point(_x, _y), density(_d) {};
	int density;
	Point point;
};

void testcase() {
  int nr_asteroids, nr_shots, nr_fighters;
  cin >> nr_asteroids >> nr_shots >> nr_fighters;
  
  int energy;
  cin >> energy;
 
  vector<Asteroid> asteroids;
  for (int i = 0; i < nr_asteroids; ++i) {
	  int x, y, d; cin >> x >> y >> d;
	  asteroids.push_back(Asteroid(x, y, d));
  }
  
  vector<Point> shots;
  for (int i = 0; i < nr_shots; ++i) {
	  int x, y;
	  cin >> x >> y;
	  shots.push_back(Point(x, y));
  }
  
  vector<Point> fighters;
  for (int i = 0; i < nr_fighters; ++i) {
	  int x, y;
	  cin >> x >> y;
	  fighters.push_back(Point(x, y));
  }
  Triangulation t;
  if (nr_fighters != 0)
	t.insert(fighters.begin(), fighters.end());
  
  Program lp (CGAL::LARGER, true, 0, false, 0);
  
  vector<vector<int>> affectedAsteroids(nr_asteroids);
  for (int i = 0; i < nr_shots; ++i) {
	  // Find min distance to fighters
	  if (nr_fighters != 0) {
		Vertex v = t.nearest_vertex(shots[i]);
	    K::FT shotRadius = CGAL::squared_distance(v->point(), shots[i]);
		for (int j = 0; j < nr_asteroids; ++j) {
		  K::FT distToAsteroid = CGAL::squared_distance(asteroids[j].point, shots[i]);
		  if (distToAsteroid < shotRadius) {
			  affectedAsteroids[j].push_back(i);
		  }
	    }
	  } else {
		 for (int j = 0; j < nr_asteroids; ++j) {
			affectedAsteroids[j].push_back(i);
		 }		 
	  }
  }
  
  for (int j = 0; j < nr_asteroids; ++j) {
	  for (auto i : affectedAsteroids[j]) {
		  ET distance = (shots[i].x() - asteroids[j].point.x())*(shots[i].x() - asteroids[j].point.x()) + (shots[i].y() - asteroids[j].point.y())*(shots[i].y() - asteroids[j].point.y());
		  if (distance < ET(1)) distance = ET(1);
		  lp.set_a(i, j, ET(1) / distance);
	  }
	  lp.set_b(j, asteroids[j].density);
  }
  
  for (int i = 0; i < nr_shots; ++i) {
	 lp.set_c(i, 1); // minimize
  }

  // solve the program, using ET as the exact type
  Solution sol = CGAL::solve_linear_program(lp, ET());
  assert (shots.solves_linear_program(lp));

  double val = to_double(sol.objective_value());
  // cout << "val = " << val << endl;
  // cout << "energy = " << energy << endl;
  // cout << sol << endl;
  if (!sol.is_infeasible() && val <= (double) energy) {
	  cout << "y" << endl;
  } else {
	  cout << "n" << endl;
  }
}

int main() {
  int t; cin >> t;
  while(t--)
	  testcase();
}