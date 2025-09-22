pipeline {
    agent none
    stages {
        stage('Build Windows') {
            agent {
                label 'windows'
            }
            steps {
                script {
                    def fallbackPath = "C:\\Program Files\\Microsoft Visual Studio\\2022\\BuildTools"

                    def vsInstallPath = bat(
                        script: '"C:\\Program Files (x86)\\Microsoft Visual Studio\\Installer\\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath',
                        returnStdout: true
                    ).trim()

                    if (!vsInstallPath) {
                        echo "vswhere did not return a path, falling back to default: ${fallbackPath}"
                        vsInstallPath = fallbackPath
                    }

                    def vcvarsall = "${vsInstallPath}\\VC\\Auxiliary\\Build\\vcvarsall.bat"

                    if (!fileExists(vcvarsall)) {
                        error "Could not find vcvarsall.bat at: ${vcvarsall}"
                    }

                    env.VC_VARSALL = vcvarsall
                    echo "Using vcvarsall at: ${vcvarsall}"
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
