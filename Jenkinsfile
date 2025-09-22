pipeline {
    agent none
    stages {
        stage('Build Windows') {
            agent {
                label 'windows'
            }
            steps {
                script {
                    def vsInstallPath = bat(
                        script: '"C:\\Program Files (x86)\\Microsoft Visual Studio\\Installer\\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath',
                        returnStdout: true
                    ).trim()

                    env.VC_VARSALL = "${vsInstallPath}\\VC\\Auxiliary\\Build\\vcvarsall.bat"
                }
                bat """
                    call "%VC_VARSALL%" amd64
                    cmake --preset msvc-release
                    cmake --build --preset msvc-release
                """ 
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
