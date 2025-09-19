pipeline {
    agent none
    stages {
        stage("Build Linux") {
            agent {
                dockerfile {
                    filename "Dockerfile.linux"
                }
            }
            steps {
                dir("build") {
                    sh "cmake -G Ninja .."
                    sh "ninja clang-format all"
                }
            }
        }
        stage("Build Android") {
            agent {
                dockerfile {
                    filename "Dockerfile.android"
                }
            }
            steps {
                dir("android") {
                    sh "gradlew buildRelease"
                }
            }
        }
    }
}
