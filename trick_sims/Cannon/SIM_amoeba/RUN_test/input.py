
execfile("Modified_data/cannon_aero.dr")
execfile("Modified_data/amoeba.py")

trick.mc_set_enabled(1)
trick.mc_set_num_runs(70)

slave0 = trick.MonteSlave("localhost")
trick_mc.mc.add_slave(slave0)

var0 = trick.MonteVarCalculated("dyn.baseball.time_to_fire_jet_1","s")
var1 = trick.MonteVarCalculated("dyn.baseball.time_to_fire_jet_2")
var2 = trick.MonteVarCalculated("dyn.baseball.time_to_fire_jet_3","s")
var3 = trick.MonteVarCalculated("dyn.baseball.time_to_fire_jet_4")

trick_mc.mc.add_variable(var0)
trick_mc.mc.add_variable(var1)
trick_mc.mc.add_variable(var2)
trick_mc.mc.add_variable(var3)

dyn.baseball.pos[0] = 0.0
dyn.baseball.pos[1] = 0.0
dyn.baseball.pos[2] = 0.0

dyn.baseball.vel[0] = 43.30
dyn.baseball.vel[1] = 0.0
dyn.baseball.vel[2] = 25.0

dyn.baseball.theta  = trick.attach_units("d" , -90.0)
dyn.baseball.phi    = trick.attach_units("d" , 1.0)
dyn.baseball.omega0 = trick.attach_units("rev/s" , 30.0)

dyn_integloop.getIntegrator(trick.Runge_Kutta_4, 6)

trick.sim_services.exec_set_terminate_time(10.0)
