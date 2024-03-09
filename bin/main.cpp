//
// Created by darkflames on 3/8/24.
//

#include <Term/PiNode.h>
#include <Term/RefNode.h>
#include <memory>

using namespace std;
using namespace term;

int main (int argc, char* argv[]) {

    auto test =  make_shared<Lambda>("a", make_shared<Idx>(1));
}