//
// Created by eugene on 20/12/2021.
//

#ifndef RUNBYRUNVERTEX__GETRUNBYRUNVERTEX_HPP_
#define RUNBYRUNVERTEX__GETRUNBYRUNVERTEX_HPP_

#include <at_task/Task.h>

class GetRunByRunVertex : public UserFillTask {

 public:
  void PreInit() override;
  void PostFinish() override;
 protected:
  bool UseATI2() const override;
  void UserInit(std::map<std::string, void *> &map) override;
  void UserExec() override;
  void UserFinish() override;

 TASK_DEF(GetRunByRunVertex, 0)
};

#endif //RUNBYRUNVERTEX__GETRUNBYRUNVERTEX_HPP_
