//
// Created by eugene on 20/12/2021.
//

#include "GetRunByRunVertex.hpp"

#include <ATI2.hpp>

#include <map>
#include <TH1.h>
#include <TGraphErrors.h>
#include <TFile.h>

using namespace ATI2;

Branch *event_header_ptr = nullptr;

Variable v_fitted_vtx;
Variable v_run_id;
Variable v_t2;
Variable v_vtx_x;
Variable v_vtx_y;
Variable v_vtx_z;

int current_run = -1;

TH1 *h_vtx_x_this_run{nullptr};
TGraphErrors *gr_vtx_x_centered{nullptr};

TH1 *h_vtx_y_this_run{nullptr};
TGraphErrors *gr_vtx_y_centered{nullptr};

std::shared_ptr<TFile> output_file{nullptr};

void fillRunInfo() {
  if (h_vtx_x_this_run) {
    gr_vtx_x_centered->SetPoint(gr_vtx_x_centered->GetN(), current_run, h_vtx_x_this_run->GetMean());
    gr_vtx_x_centered->SetPointError(gr_vtx_x_centered->GetN()-1, 0., h_vtx_x_this_run->GetStdDev());
  }

  if (h_vtx_y_this_run) {
    gr_vtx_y_centered->SetPoint(gr_vtx_y_centered->GetN(), current_run, h_vtx_y_this_run->GetMean());
    gr_vtx_y_centered->SetPointError(gr_vtx_y_centered->GetN()-1, 0., h_vtx_y_this_run->GetStdDev());
  }

}

void writeHistograms() {
  if (h_vtx_x_this_run) {
    output_file->WriteObject(h_vtx_x_this_run, h_vtx_x_this_run->GetName());
  }
  if (h_vtx_y_this_run) {
    output_file->WriteObject(h_vtx_y_this_run, h_vtx_y_this_run->GetName());
  }
}


bool GetRunByRunVertex::UseATI2() const {
  return true;
}
void GetRunByRunVertex::PreInit() {
  UserTask::PreInit();
}
void GetRunByRunVertex::UserInit(std::map<std::string, void *> &map) {
  config_->Print();

  event_header_ptr = GetInBranch("RecEventHeader");

  GetInBranch("RecEventHeader")->UseFields({
    {"fitted_vtx", v_fitted_vtx},
    {"run_id", v_run_id},
    {"t2", v_t2},
    {"vtx_x", v_vtx_x},
    {"vtx_y", v_vtx_y},
    {"vtx_z", v_vtx_z},
  });

  output_file = std::make_shared<TFile>("qa.root", "recreate");
  gr_vtx_x_centered = new TGraphErrors;
  gr_vtx_y_centered = new TGraphErrors;

}
void GetRunByRunVertex::UserExec() {
  using std::cout;
  using std::endl;
  auto &event_header_ref = *event_header_ptr;

  auto run_id = event_header_ref[v_run_id].GetInt();

  if (
      event_header_ref[v_t2].GetBool() &&
      event_header_ref[v_fitted_vtx].GetBool() &&
      -594.0 < event_header_ref[v_vtx_z] && event_header_ref[v_vtx_z] < -590.0) {

    if (current_run != run_id) {
      fillRunInfo();
      writeHistograms();

      // init histogram
      h_vtx_x_this_run = new TH1D(Form("h_vtx_x_run%d", run_id),
                                                ";x (cm)", 200, -1.0, 1.0);
      h_vtx_x_this_run->SetTitle(Form("Run #%d", run_id));

      h_vtx_y_this_run = new TH1D(Form("h_vtx_y_run%d", run_id),
                                  ";y (cm)", 200, -1.0, 1.0);
      h_vtx_y_this_run->SetTitle(Form("Run #%d", run_id));

      current_run = run_id;
    }

    h_vtx_x_this_run->Fill(event_header_ref[v_vtx_x]);
    h_vtx_y_this_run->Fill(event_header_ref[v_vtx_y]);
  }


}
void GetRunByRunVertex::UserFinish() {
  fillRunInfo();
  writeHistograms();

  output_file->WriteObject(gr_vtx_x_centered, "gr_vtx_x");
  output_file->WriteObject(gr_vtx_y_centered, "gr_vtx_y");

  output_file.reset();
}
void GetRunByRunVertex::PostFinish() {
  UserTask::PostFinish();
}

TASK_IMPL(GetRunByRunVertex)