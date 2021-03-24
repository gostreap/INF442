#include <algorithm> // for count
#include <cassert>
#include <cfloat>  // for DBL_MAX
#include <cmath>   // for sqrt, fabs
#include <cstdio>  // for EOF
#include <cstdlib> // for rand, srand
#include <ctime>   // for rand seed
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <vector>

#include <gtkmm/application.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>

class point {
  public:
    static int d;
    double *coords;
    int label;

    point() {
        coords = new double[d]{0};
        label = 0;
    }

    ~point() { delete[] coords; }

    void print() {
        for (int i = 0; i < d; i++) {
            std::cout << coords[i] << ((i == d - 1) ? '\n' : '\t');
        }
    }

    double squared_dist(point &q) {
        double sd = 0;
        for (int i = 0; i < d; i++) {
            sd += (coords[i] - q.coords[i]) * (coords[i] - q.coords[i]);
        }
        return sd;
    }
};

int point::d;

void test_point() {
    point::d = 3;
    point q1, q2;
    q1.print();
    q2.print();
    q1.coords[0] = 1;
    q1.coords[1] = 1;
    q1.coords[2] = 1;
    q1.print();
    std::cout << q1.squared_dist(q2) << std::endl;
    q2.coords[0] = 3;
    q2.coords[1] = 4;
    q2.coords[2] = 5;
    q2.print();
    std::cout << q1.squared_dist(q2) << std::endl;
}

class cloud {
  private:
    int d;
    int n;
    int k;

    // maximum possible number of points
    int nmax;

    point *points;
    point *centers;

  public:
    cloud(int _d, int _nmax, int _k) {
        d = _d;
        point::d = _d;
        n = 0;
        k = _k;

        nmax = _nmax;

        points = new point[nmax];
        centers = new point[k];
    }

    ~cloud() {
        delete[] centers;
        delete[] points;
    }

    void add_point(point &p, int label) {
        assert(("[NOK]", n < nmax));

        for (int m = 0; m < d; m++) {
            points[n].coords[m] = p.coords[m];
        }

        points[n].label = label;

        n++;
    }

    int get_d() { return d; }

    int get_n() { return n; }

    int get_k() { return k; }

    point &get_point(int i) { return points[i]; }

    point &get_center(int j) { return centers[j]; }

    void set_center(point &p, int j) {
        for (int m = 0; m < d; m++)
            centers[j].coords[m] = p.coords[m];
    }

    double intracluster_variance() {
        double var = 0;
        for (int i = 0; i < nmax; i++) {
            var += points[i].squared_dist(centers[points[i].label]);
        }
        return var / nmax;
    }

    int set_voronoi_labels() {
        int count = 0;
        double dist;
        for (int i = 0; i < nmax; i++) {
            dist = points[i].squared_dist(centers[points[i].label]);
            for (int j = 0; j < k; j++) {
                if (dist > points[i].squared_dist(centers[j])) {
                    count++;
                    points[i].label = j;
                }
            }
        }
        return count;
    }

    void set_centroid_centers() {
        double centers_coord[k][d] = {0};
        int count[k] = {0};
        for (int i = 0; i < nmax; i++) {
            count[points[i].label] += 1;
            for (int j = 0; j < d; j++) {
                centers_coord[points[i].label][j] += points[i].coords[j];
            }
        }
        for (int i = 0; i < k; i++) {
            if (count[i] != 0) {
                for (int j = 0; j < d; j++) {
                    centers[i].coords[j] = centers_coord[i][j] / count[i];
                }
            }
        }
    }

    void kmeans() {
        init_forgy();

        int change = 1;
        while (change != 0) {
            change = set_voronoi_labels();
            set_centroid_centers();
        }
    }

    void init_random() {

        double max = 0;

        for (int i = 0; i < nmax; i++) {
            for (int j = 0; j < d; j++) {
                if (max < points[i].coords[j]) {
                    max = points[i].coords[j];
                }
            }
        }

        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_real_distribution<double> distr(-max, max);

        for (int i = 0; i < k; i++) {
            for (int j = 0; j < d; j++) {
                centers[i].coords[j] = distr(eng);
            }
        }
    }

    void init_forgy() {
        int r, i = 0;
        std::set<int> sample;

        while (sample.size() != k) {
            r = rand() % nmax;
            if (sample.find(r) == sample.end()) {
                for (int j = 0; j < d; j++) {
                    centers[i].coords[j] = points[r].coords[j];
                }
                sample.insert(r);
                i++;
            }
        }
    }

    void init_plusplus() {
        int r = rand() % nmax;
        for (int j = 0; j < d; j++) {
            centers[0].coords[j] = points[r].coords[j];
        }

        double rd;
        double sum;
        double dist[nmax];
        double total_dist;
        for (int i = 1; i < k; i++) {
            total_dist = 0;
            for (int j = 0; j < nmax; j++) {
                dist[j] = points[j].squared_dist(centers[0]);
                for (int l = 1; l < i; l++) {
                    dist[j] = std::min(points[j].squared_dist(centers[l]), dist[j]);
                }
                total_dist += dist[j];
            }
            rd = (double)rand() / RAND_MAX;
            sum = 0;
            r = -1;
            for (int j = 0; j < nmax; j++) {
                r++;
                sum += dist[j] / total_dist;
                if (rd < sum) {
                    break;
                }
            }

            for (int j = 0; j < d; j++) {
                centers[i].coords[j] = points[r].coords[j];
            }
        }
    }

    void init_random_partition() {
        for (int i = 0; i < nmax; i++) {
            points[i].label = rand() % k;
        }
    }

    void silhouette() {
		int MAX = k;
        double a[MAX];
        int count[MAX];
        double min, max;
        double total = 0;

        for (int i = 1; i < MAX; i++) {
			k = i;
            kmeans();
            total = 0;
            for (int j = 0; j < nmax; j++) {
                // std::cerr << "####" << std::endl;
                for (int l = 0; l < i; l++) {
                    a[l] = 0;
                    count[l] = 0;
                }
                for (int l = 0; l < nmax; l++) {
                    if (l != j) {
                        count[points[l].label]++;
                        a[points[l].label] += points[j].squared_dist(points[l]);
                    }
                }
                min = DBL_MAX;
                for (int l = 0; l < i; l++) {
                    // std::cerr << "count " << l << " - " << count[l] << " " << a[l] << std::endl;
                    a[l] = ((count[l] != 0) ? (a[l] / count[l]) : DBL_MAX);
                    // std::cerr << "val " << l << " - " << a[l] << std::endl;
                    if (l != points[j].label && a[l] != 0) {
                        min = std::min(min, a[l]);
                    }
                }
                max = std::max(min, a[points[j].label]);
                // std::cerr << min << " " << a[points[j].label] << " " << max << " -> " << (min - a[points[j].label]) / max << std::endl;
                total += (min - a[points[j].label]) / max;
            }
            std::cerr << i << " - " << total << " " << total / nmax << std::endl;
            total = total / nmax;
        }
    }
};

// test functions
void test_intracluster_variance() {
    // tolerance for comparison of doubles
    const double eps = 0.0001;

    // dimension used for tests
    point::d = 1;

    // temporary container
    point p;

    std::cout << "Testing function intracluster_variance()...";

    // test case 1
    const double dist_onepoint_zerodist = 0.0;
    cloud onepoint_zerodist(1, 1, 1);
    p.coords[0] = 0.0;
    onepoint_zerodist.add_point(p, 0);
    assert(("[NOK]", std::fabs(onepoint_zerodist.intracluster_variance() - dist_onepoint_zerodist) < eps));

    // test case 2
    const double dist_onepoint_posdist = 0.25;
    cloud onepoint_posdist(1, 1, 1);
    p.coords[0] = 0.5;
    onepoint_posdist.add_point(p, 0);
    assert(("[NOK]", std::fabs(onepoint_posdist.intracluster_variance() - dist_onepoint_posdist) < eps));

    // test case 3
    const double dist_twopoints = 0.625;
    cloud twopoints(1, 2, 1);
    p.coords[0] = -1.0;
    twopoints.add_point(p, 0);
    p.coords[0] = 0.5;
    twopoints.add_point(p, 0);
    p.coords[0] = -0.5;
    twopoints.set_center(p, 0);
    assert(("[NOK]", std::fabs(twopoints.intracluster_variance() - dist_twopoints) < eps));

    // test case 4
    const double dist_twoclusters = 6.8125;
    cloud twoclusters(1, 4, 2);
    p.coords[0] = -1.0;
    twoclusters.add_point(p, 0);
    p.coords[0] = 0.5;
    twoclusters.add_point(p, 0);
    p.coords[0] = -0.5;
    twoclusters.set_center(p, 0);
    p.coords[0] = -2.0;
    twoclusters.add_point(p, 1);
    p.coords[0] = 2.0;
    twoclusters.add_point(p, 1);
    p.coords[0] = -3.0;
    twoclusters.set_center(p, 1);
    assert(("[NOK]", std::fabs(twoclusters.intracluster_variance() - dist_twoclusters) < eps));

    std::cout << "\t[OK]" << std::endl;
}

void test_set_voronoi_labels() {
    std::cout << "Testing function set_voronoi_labels()...";
    point::d = 1;
    point p;
    int change, res;

    // test case 1
    change = 1;
    cloud cloud1(1, 2, 2);
    p.coords[0] = -1.0;
    cloud1.add_point(p, 0);
    p.coords[0] = 1;
    cloud1.add_point(p, 0);
    p.coords[0] = -0.5;
    cloud1.set_center(p, 0);
    p.coords[0] = 0.5;
    cloud1.set_center(p, 1);
    res = cloud1.set_voronoi_labels();
    assert(("[NOK]", change == res));

    // test case 2
    change = 2;
    cloud cloud2(1, 2, 2);
    p.coords[0] = 2;
    cloud2.add_point(p, 0);
    p.coords[0] = 1;
    cloud2.add_point(p, 0);
    p.coords[0] = -0.5;
    cloud2.set_center(p, 0);
    p.coords[0] = 0.5;
    cloud2.set_center(p, 1);
    res = cloud2.set_voronoi_labels();
    assert(("[NOK]", change == res));

    std::cout << "\t[OK]" << std::endl;
}

void test_set_centroid_centers() {
    std::cout << "Testing function set_centroid_centers()...";
    // tolerance for comparison of doubles
    const double eps = 0.0001;
    point::d = 1;
    point p;
    int change, res;

    // test case 1
    point center1, center2;
    center1.coords[0] = -1;
    center2.coords[0] = 1;
    cloud cloud1(1, 2, 2);
    p.coords[0] = -1.0;
    cloud1.add_point(p, 0);
    p.coords[0] = 1;
    cloud1.add_point(p, 0);
    p.coords[0] = -0.5;
    cloud1.set_center(p, 0);
    p.coords[0] = 0.5;
    cloud1.set_center(p, 1);
    cloud1.set_voronoi_labels();
    cloud1.set_centroid_centers();
    assert(("[NOK]",
            std::fabs(center1.squared_dist(cloud1.get_center(0)) + center2.squared_dist(cloud1.get_center(1)) < eps)));

    std::cout << "\t[OK]" << std::endl;
}

void test_kmeans() {
    // TODO
}

void test_init_forgy() {
    // number of random experiments
    const int K = 10000;
    // tolerance in probability
    const double delta = 0.0625;

    // dimenstion used for tests
    point::d = 1;

    // temporary container
    point p;

    std::cout << "Testing function init_forgy()...";

    const double prob_threepoints = 0.3333;
    cloud threepoints(1, 3, 1);
    p.coords[0] = 0.0;
    threepoints.add_point(p, 0);
    p.coords[0] = 1.0;
    threepoints.add_point(p, 0);
    p.coords[0] = 2.0;
    threepoints.add_point(p, 0);
    int cnt = 0;
    for (int k = 0; k < K; k++) {
        threepoints.init_forgy();
        if (threepoints.get_center(0).coords[0] == 1.0)
            cnt++;
    }
    assert(("[NOK]", std::fabs(cnt / (double)K - prob_threepoints) < delta));

    std::cout << "\t\t[OK]" << std::endl;
}

void test_init_plusplus() {
    // number of random experiments
    const int K = 10000;
    // tolerance in probability
    const double delta = 0.0625;

    // dimenstion used for tests
    point::d = 1;

    // temporary container
    point p;

    std::cout << "Testing function init_plusplus()...";

    // test case 1
    const double prob_threepoints = 0.3333;
    cloud threepoints(1, 3, 1);
    p.coords[0] = 0.0;
    threepoints.add_point(p, 0);
    p.coords[0] = 1.0;
    threepoints.add_point(p, 0);
    p.coords[0] = 2.0;
    threepoints.add_point(p, 0);
    int cnt = 0;
    for (int k = 0; k < K; k++) {
        threepoints.init_plusplus();
        if (threepoints.get_center(0).coords[0] == 1.0)
            cnt++;
    }
    assert(("[NOK]", std::fabs(cnt / (double)K - prob_threepoints) < delta));

    // test case 2
    const double prob_twoclusters = 0.125;
    cloud twoclusters(1, 4, 2);
    p.coords[0] = 0.0;
    twoclusters.add_point(p, 0);
    p.coords[0] = 0.0;
    twoclusters.add_point(p, 0);
    p.coords[0] = 1.0;
    twoclusters.add_point(p, 0);
    p.coords[0] = 2.0;
    twoclusters.add_point(p, 0);
    cnt = 0;
    for (int k = 0; k < K; k++) {
        twoclusters.init_plusplus();
        if (twoclusters.get_center(1).coords[0] == 1.0)
            cnt++;
    }
    assert(("[NOK]", std::fabs(cnt / (double)K - prob_twoclusters) < delta));

    std::cout << "\t\t[OK]" << std::endl;
}

void test_init_random_partition() {
    // number of random experiments
    const int K = 10000;
    // tolerance in probability
    const double delta = 0.0625;

    // dimenstion used for tests
    point::d = 1;

    // temporary container
    point p;

    std::cout << "Testing function init_random_parition()...";

    const double prob_threepoints = 0.3333;
    cloud threepoints(1, 3, 3);
    p.coords[0] = 0.0;
    threepoints.add_point(p, 0);
    p.coords[0] = 1.0;
    threepoints.add_point(p, 0);
    p.coords[0] = 2.0;
    threepoints.add_point(p, 0);
    int cnt = 0;
    for (int k = 0; k < K; k++) {
        threepoints.init_random_partition();
        if (threepoints.get_point(2).label == 1)
            cnt++;
    }
    assert(("[NOK]", std::fabs(cnt / (double)K - prob_threepoints) < delta));

    std::cout << "\t[OK]" << std::endl;
}

// for graphical interface
class MyArea : public Gtk::DrawingArea {
  private:
    cloud *c;
    int x_column;
    int y_column;

  public:
    MyArea(cloud *_c, int _x_column, int _y_column) {
        c = _c;
        x_column = _x_column;
        y_column = _y_column;
    }

    virtual ~MyArea() {}

  protected:
    // Override default signal handler:
    bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;
};

/**
 * Counts the number of tab-separated columns in the given line.
 */
int nb_columns(const std::string &line) { return std::count(line.begin(), line.end(), '\t') + 1; }

int main(int argc, char **argv) {
    bool run_tests = false;
    // test_point();

    if (argc < 5 || argc > 6) {
        std::cerr << "Usage: " << argv[0] << " csv nb_clusters x_column y_column [ test ]" << std::endl;
        std::exit(1);
    }
    std::string csv_filename = argv[1];
    int nb_clusters = std::stoi(argv[2]);
    int x_column = std::stoi(argv[3]);
    int y_column = std::stoi(argv[4]);

    if (argc >= 6)
        run_tests = true;

    srand(time(NULL));

    if (run_tests) {
        test_intracluster_variance();
        test_set_voronoi_labels();
        test_set_centroid_centers();
        test_kmeans();
        test_init_forgy();
        test_init_plusplus();
        test_init_random_partition();
    }

    // open data file
    std::ifstream is(csv_filename);
    assert(is.is_open());

    // read header line
    std::string header_line;
    std::getline(is, header_line);
    std::vector<std::string> names;

    const int d = nb_columns(header_line) - 1;
    const int nmax = 150;
    const int k = nb_clusters;

    // construct point cloud
    cloud c(d, nmax, k);

    // point to read into
    point p;

    // labels to cycle through
    int label = 0;

    // while not at end of file
    while (is.peek() != EOF) {
        // read new points
        for (int m = 0; m < d; m++) {
            is >> p.coords[m];
        }

        c.add_point(p, label);

        label = (label + 1) % k;

        // read ground-truth labels
        // unused in normal operation
        std::string next_name;
        is >> next_name;
        names.push_back(next_name);

        // consume \n
        is.get();
    }

    // execute k-means algorithm
    for (int i = 0; i < 1; i++) {

        std::cout << "Intracluster variance before k-means: " << c.intracluster_variance() << std::endl;
        c.kmeans();
        c.silhouette();
        std::cout << "Intracluster variance after k-means: " << c.intracluster_variance() << std::endl;
    }

    std::cout << "Saving clustering into \"output.csv\"... ";
    std::ofstream os("output.csv");
    assert(os.is_open());
    os << header_line << '\n';
    for (int i = 0; i < c.get_n(); ++i) {
        for (int j = 0; j < c.get_d(); ++j) {
            os << c.get_point(i).coords[j] << '\t';
        }
        os << names[i] << "_Label_" << c.get_point(i).label;
        if (i != c.get_n() - 1)
            os << '\n';
    }
    std::cout << "done" << std::endl;

    // launch graphical interface
    int gtk_argc = 1;
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(gtk_argc, argv, "inf442.td3");

    Gtk::Window win;
    win.set_title("TD 3");
    win.set_default_size(400, 400);

    MyArea area(&c, x_column, y_column);
    win.add(area);
    area.show();

    return app->run(win);
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    // find min and max on each axis
    double x_min = DBL_MAX;
    double x_max = DBL_MIN;
    double y_min = DBL_MAX;
    double y_max = DBL_MIN;
    for (int i = 0; i < c->get_n(); i++) {
        if (c->get_point(i).coords[x_column] < x_min)
            x_min = c->get_point(i).coords[x_column];

        if (c->get_point(i).coords[x_column] > x_max)
            x_max = c->get_point(i).coords[x_column];

        if (c->get_point(i).coords[y_column] < y_min)
            y_min = c->get_point(i).coords[y_column];

        if (c->get_point(i).coords[y_column] > y_max)
            y_max = c->get_point(i).coords[y_column];
    }

    // plot all points
    for (int i = 0; i < c->get_n(); i++) {
        cr->save(); // save current drawing context (opaque black)
        cr->arc((c->get_point(i).coords[x_column] - x_min) * width / (x_max - x_min),
                (c->get_point(i).coords[y_column] - y_min) * height / (y_max - y_min), 10.0, 0.0,
                2.0 * M_PI); // full circle

        // choose color depending on label
        switch (c->get_point(i).label) {
            case 0:
                cr->set_source_rgba(1.0, 0.0, 0.0, 0.6); // red, partially translucent
                break;

            case 1:
                cr->set_source_rgba(0.0, 0.0, 0.8, 0.6); // 0.8 blue, partially translucent
                break;

            case 2:
                cr->set_source_rgba(0.0, 1.0, 0.0, 0.6); // green, partially translucent
                break;

            default:
                double shade = c->get_point(i).label / (double)c->get_k();
                cr->set_source_rgba(shade, shade, shade, 1.0); // gray
                break;
        }

        cr->fill_preserve();
        cr->restore(); // restore drawing context to opaque black
        cr->stroke();
    }

    return true;
}
