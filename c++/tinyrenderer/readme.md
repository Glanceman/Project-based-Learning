 ![image](./output.tga)

## Compile
```mingw32-make```



## Remark

### Line5($x_0$, $y_0$, $x_1$, $y_1$, Color)
 ![image](/public/Bresenham’s%20Line%20Drawing%20Algorithm.png)



### barycentric-coordinates
$$
    px = w_A * ax + w_B * bx + w_C * cx \\
    py = w_A * ay + w_B * by + w_C * cy \\
    1  = w_A + w_B + w_C\\
$$

$$
\begin{pmatrix}
p_x \\
p_y \\
1
\end{pmatrix}
=
\begin{pmatrix}
a_x & b_x & c_x \\
a_y & b_y & c_y \\
1 & 1 & 1
\end{pmatrix}
\begin{pmatrix}
w_A \\
w_B \\
w_C
\end{pmatrix}
$$

## Reference
1. https://github.com/ssloy/tinyrenderer 