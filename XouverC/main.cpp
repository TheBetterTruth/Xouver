#include "parser.h"

int main() {
    parser p("Test", "Xouver", "func main() { var x = 5 + 1 }");
    p.parse();

    return 0;
}