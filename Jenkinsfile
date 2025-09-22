pipeline {
    agent none
    stages {
        stage('Build Windows') {
            agent {
                label 'windows'
            }
            steps {
                dir('build') {
                    bat 'cmake -G Ninja ..'
                    bat 'ninja'
                }
            }
        }
        stage('Build Linux') {
            agent {
                dockerfile {
                    filename 'Dockerfile.linux'
                }
            }
            steps {
                dir('build') {
                    sh 'cmake -G Ninja ..'
                    sh 'ninja clang-format all'
                }
            }
        }
        stage('Build Android') {
            agent {
                dockerfile {
                    filename 'Dockerfile.android'
                }
            }
            environment {
                KEYSTORE = credentials('release-keystore')
                KEYSTORE_PASSWORD = credentials('release-keystore-password')
            }
            steps {
                dir('android') {
                    sh 'gradlew assembleRelease'
                }
                archiveArtifacts artifacts: 'android/app/build/outputs/apk/*/*.apk', fingerprint: true
            }
        }
    }
}
