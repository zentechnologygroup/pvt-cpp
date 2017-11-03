Quantity<SCF_STB> Rsp1__correlation__fct(const Quantity<STB_MMscf>& ogr) {
  return Rsp1::get_instance()(ogr);
}

double Rsp1__correlation__valid__fct(const double& ogr) {
  return Rsp1::get_instance()(ogr).raw();
}
