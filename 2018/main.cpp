#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <bitset>
#include <cstdio>
#define X first
#define Y second
#define constDist 10
#define ERR 1e-6

using namespace std;

int R, C, F, N, B, T;
typedef pair<int, int> point;

struct cursa {

    point startPoint, endPoint;
    int startTime, endTime;

    cursa(point sp, point ep, int sT, int eT) {

        startPoint = sp;
        endPoint = ep;
        startTime = sT;
        endTime = eT;
    }

    inline int distCursa() const {

        return (int) abs(startPoint.X - endPoint.X) + (int) abs(startPoint.Y - endPoint.Y);
    }

    inline int distInterv() const {

        return endTime - startTime;
    }
};

struct masina {

    vector<int> curseAsoc;
    point Pos;
    int disponTime;
    bool canBeUsed;

    masina() {

        Pos = make_pair(0, 0);
        disponTime = 0;
        canBeUsed = true;
    }
};

vector<pair<cursa, int>> curse;
vector<masina> masini;

inline int distCursaManh(const point& p1, const point &p2) {

    return (int) abs(p1.X - p2.X) + (int) abs(p1.Y - p2.Y);
}

void read() {

    ifstream in("hashcode.in");
    in >> R >> C >> F >> N >> B >> T;

    int a, b, x, y, s, f;

    for (int i = 1; i <= F; i++)
        masini.push_back(masina());

    for (int i = 1; i <= N; i++) {

        in >> a >> b >> x >> y >> s >> f;

        //cout << a << " " << b << " ";
        //cout << x << " " << y << "\n";

        if (distCursaManh(point(0, 0), point(a, b)) + distCursaManh(point(a, b), point(x, y)) <= T)
            curse.push_back(make_pair(cursa(point(a, b), point(x, y), s, f), i - 1));
    }
    in.close();
}

bool compare(const pair<cursa,int>& c1, const pair<cursa, int>& c2) {

    double score1 = c1.first.distCursa() / (static_cast<double> (c1.first.distInterv() * c1.first.endTime));
    double score2 = c2.first.distCursa() / (static_cast<double> (c2.first.distInterv() * c2.first.endTime));

    if (score1 >= score2)
        return true;

    return false;
}

int main() {

    read();

    //sort(curse.begin(), curse.end(), compare);

    int globalScore, distRel, indexMasina;

    for (unsigned int i = 0; i < curse.size(); i++) {

        globalScore = 0;
        distRel = T + 1;
        indexMasina = -1;

        for (unsigned int j = 0; j < masini.size(); j++) {

            int allDist = distCursaManh(masini[j].Pos, curse[i].first.startPoint);
            allDist += curse[i].first.distCursa();
            allDist += masini[j].disponTime;

            if (allDist < T) {

                int scoreCursa = max(0, curse[i].first.distCursa() - max(0, (curse[i].first.startTime - (masini[j].disponTime + distCursaManh(masini[j].Pos, curse[i].first.startPoint)))));

                if (distCursaManh(masini[j].Pos, curse[i].first.startPoint) < distRel && scoreCursa >= globalScore) {

                    indexMasina = j;
                    globalScore = scoreCursa;
                    distRel = distCursaManh(masini[j].Pos, curse[i].first.startPoint);
                }
            }
        }

        if (indexMasina >= 0) {

            masini[indexMasina].disponTime += distCursaManh(masini[indexMasina].Pos, curse[i].first.startPoint) + curse[i].first.distCursa();
            masini[indexMasina].Pos = curse[i].first.endPoint;
            masini[indexMasina].curseAsoc.push_back(curse[i].second);
        }
    }

    ofstream out("hashcode.out");
    for (unsigned int i = 0; i < masini.size(); i++) {

        out << masini[i].curseAsoc.size() << " ";

        for (unsigned int j = 0; j < masini[i].curseAsoc.size(); j++)
            out << masini[i].curseAsoc[j] << " ";
        out << "\n";
    }
    out.close();

    return 0;
}
