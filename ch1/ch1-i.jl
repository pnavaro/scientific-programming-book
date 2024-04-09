using Printf

function main()
  x = 0.0      # initialize position
  y = 2.1
  vx = 45.     # initialize velocity
  vy = 0.
  gx = 0.      # initialize acceleration
  gy = -9.81
  dt = 0.04    # assign time step size
  n = 0        # time step index

  #show initial values
  @printf "%.2f, %.2f, %.2f, %.2f, %.2f\n" n*dt x y vx vy

  # rewind by half time step for Leapfrog
  vx -= 0.5*gx*dt;
  vy -= 0.5*gy*dt;

  while y>0     # repeat until ground impact
      vx += gx*dt # increment velocity to n+0.5
      vy += gy*dt
      x += vx*dt  # increment position
      y += vy*dt
      n +=1         # increment time step counter
      # display current position and velocity
      @printf "%.2f, %.2f, %.2f, %.2f, %.2f\n" n*dt x y vx vy
  end
  return 0
end
