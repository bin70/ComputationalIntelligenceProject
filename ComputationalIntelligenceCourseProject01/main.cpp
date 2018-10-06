#include "SeparateOil.hpp"
#include <ctime>

int main()
{
    clock_t start, finish;
    start = clock();

    int s_b, s_m, s_s, t_b, t_m, t_s;
    std::cout << "输入起始状态三个油瓶的油量，以空格分隔：" << std::endl;
    std::cin >> s_b >> s_m >> s_s;
    std::cout << "输入目标状态三个油瓶的油量，以空格分隔：" << std::endl;
    std::cin >> t_b >> t_m >> t_s;
    
    SeparateOil spo;
    if(!spo.run(s_b, s_m, s_s, t_b, t_m, t_s))
    {
        std::cout << "不存在从起始状态到目标状态的分油方案!"<< std::endl;
    }

    finish = clock();
    std::cout << "运行时间：" << (double)
(finish-start)/CLOCKS_PER_SEC << "s." << std::endl;    return 0;
}