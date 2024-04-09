function fun(x::T) where T
  z = x*x - 2x
  return z>=0 ? z : zero(T)
end

function main(n)
  y = zeros(n)
  for i in eachindex(y)
    x = -1.0 + 2i/(n-1)
    y[i] = fun(x)
  end

  println("f(-1)=",y[begin], ", f(1)=", y[end])
  return 0;
end
 
n = 11
main(n)
