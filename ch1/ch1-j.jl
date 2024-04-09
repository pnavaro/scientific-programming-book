using Plots

"""
 Julia version of a tennis ball simulation 
 Brieda, Wang, Martin, "Introduction to Modern Scientific Programming and Numerical Methods", CRC Press 2021
"""

function main(nn)

  x = Float64[]
  y = Float64[]
  # set initial values
  x = push!(x, 0)
  y = push!(y, 2.1)
  vx = 45     
  vy = 0

  # additional parameters
  gx = 0.0      
  gy = -9.81
  dt = 0.04   

  # rewind by half time step for Leapfrog
  vx -= 0.5*gx*dt
  vy -= 0.5*gy*dt

  anim = @animate for n = 1:nn
    vx = vx + gx*dt  # vx[i+1] is now at t+0.5dt
    vy = vy + gy*dt
    push!(x, x[end] + vx*dt)
    push!(y, y[end] + vy*dt)
    if y[end] < 0  # check for impact
        y[end] *= -1.0
        alpha = 0.5 # bounciness 
        vy *= -alpha;
        vx *= alpha;  
    end
    scatter( x, y, ms = 12, xlims = (0,60), ylims = (0,2.5), label="", alpha = 1 - n / nn )
  end

  return anim

end

nn = 100 # number of iterations

anim = main(nn)

gif(anim, "ball.gif", fps = 15)

