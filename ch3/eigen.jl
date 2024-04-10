"""
ASTE-404 Eigenvalue demo
@author: lubos
"""

using LinearAlgebra

A = [0 2 0; 2 0 0; 1 0 1]
lam, vec = eigen(A)

for i in eachindex(lam)
    print("lambda=$(lam[i]), vec=")
    local v = vec[:,i]
    println(v)
    println(A * v)
    println(lam[i]*v)
    println("****")
end
        
