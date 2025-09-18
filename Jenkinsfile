pipeline {
    agent { dockerfile true }
    stages {
        stage("Build") {
            steps {
                dir("build") {
                    sh 'cmake -G Ninja ..'
                    sh 'ninja clang-format all'
                }
            }
        }
    }
}
