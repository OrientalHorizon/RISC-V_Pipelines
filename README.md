# RISC-V_Pipelines

五级流水图：![image-20230703112123245](C:\Users\iakno\AppData\Roaming\Typora\typora-user-images\image-20230703112123245.png)

Forwarding: 在 Reg 阶段检查 forwarding line，可以读到上上条指令执行后相应寄存器的值；

在 ALU 阶段检查 forwarding line，检查上条指令有没有改变要用到的寄存器。

分支预测：在 Ifetch 阶段做，记录预测错误时 `pc` 应该是什么值。如果预测失败，清空流水线并且改正 `pc`。

预测准确率：

| 测试点             | 准确率 |
| ------------------ | ------ |
| **array_test1**    | 47.8%  |
| **array_test2**    | 48.1%  |
| **basicopt1**      | 55.9%  |
| **bulgarian**      | 56.4%  |
| **expr**           | 57.1%  |
| **gcd**            | 55.2%  |
| **hanoi**          | 50.0%  |
| **lvalue2**        | 37.5%  |
| **magic**          | 30.1%  |
| **manyarguments**  | 70.0%  |
| **multiarray**     | 79.8%  |
| **naive**          | NaN    |
| **pi**             | 58.9%  |
| **qsort**          | 68.8%  |
| **queens**         | 57.6%  |
| **statement_test** | 54.1%  |
| **superloop**      | 78.2%  |
| **tak**            | 73.8%  |

一些测试点预测准确率不高的原因：分支预测采用了全局的方式。