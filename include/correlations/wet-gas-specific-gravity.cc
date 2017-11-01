Quantity<Sgg> YwgrMcCain__correlation__fct(const Quantity<Sgg>& yg,
                                           const Quantity<Sg_do>& yo,
                                           const Quantity<SCF_STB>& rsp1,
                                           const Quantity<SCF_STB>& gpa,
                                           const Quantity<SCF_STB>& veq) {
  return YwgrMcCain::get_instance()(yg, yo, rsp1, gpa, veq);
}

double YwgrMcCain__correlation__valid__fct(const double& yg, const double& yo,
                                           const double& rsp1,
                                           const double& gpa,
                                           const double& veq) {
  return YwgrMcCain::get_instance()(yg, yo, rsp1, gpa, veq).raw();
}
