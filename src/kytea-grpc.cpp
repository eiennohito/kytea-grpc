//
// Created by Arseny Tolmachev on 2016/10/13.
//

#include <kytea/kytea.h>

#include <iostream>
#include <mutex>

#include <kytea/string-util.h>
#include "kytea-svc.grpc.pb.h"

#include <grpc++/grpc++.h>

class KyteaGrpcServerImpl final : public kytea::grpc::KyteaSvc::Service {
private:
  kytea::Kytea& core_;
  kytea::StringUtil *util_;
  kytea::KyteaConfig *cfg_;
  std::mutex mutex_;

public:
  KyteaGrpcServerImpl(kytea::Kytea& kt): core_(kt), util_(kt.getStringUtil()), cfg_(kt.getConfig()) {}

  grpc::Status Analyze(grpc::ServerContext* ctx,
                       const kytea::grpc::KyteaRequest* req,
                       kytea::grpc::KyteaSentence* resp) override {

    kytea::KyteaString str;
    kytea::KyteaString norm;

    {
      std::lock_guard<std::mutex> lock_{mutex_};
      str = util_->mapString(req->input());
      norm = util_->normalize(str);
    }

    kytea::KyteaSentence sentence { str, norm };

    core_.calculateWS(sentence);

    auto num_tags = cfg_->getNumTags();

    for (int i = 0; i < num_tags; ++i) {
      core_.calculateTags(sentence, i);
    }
    
    const auto& words = sentence.words;
    
    for (auto &w : words) {
      auto u = resp->add_units();
      auto impl = w.surface.getImpl();
      auto data = util_->showString(w.surface);
      u->add_parts(data);

      for (int i = 0; i < w.getNumTags(); ++i) {
        auto tagData = util_->showString(w.getTag(i)->first);
        u->add_parts(tagData);
      }
    }

    return grpc::Status::OK;
  }
};

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "You should specify model path as a parameter\n";
    return 1;
  }

  kytea::Kytea core;

  std::cout << "reading model...";
  core.readModel(argv[1]);
  std::cout << "Done!\n";

  KyteaGrpcServerImpl svc {core};

  grpc::ServerBuilder bldr;

  bldr.RegisterService(&svc);

  std::string listen_address("0.0.0.0:56232");
  int port;
  bldr.AddListeningPort(listen_address, grpc::InsecureServerCredentials(), &port);

  auto srv = bldr.BuildAndStart();

  std::cout << "started grpc-kytea on " << listen_address << " using port " << port;

  srv->Wait();

  return 0;
}