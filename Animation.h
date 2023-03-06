#pragma once

#include "Math.h"
#include "Time.h"

#include <functional>

namespace Easing {
using Function = std::function<float(float, float, Seconds)>;

float apply(Function f, float start, float end, Seconds alpha) { return f(start, end, alpha); }

Vector3 apply(Function f, const Vector3& start, const Vector3& end, Seconds alpha) {
    return {f(start.x, end.x, alpha), f(start.y, end.y, alpha), f(start.z, end.z, alpha)};
}

const Function linear = [](float start, float end, Seconds alpha) {
    return start + (end - start) * alpha.value;
};

} // namespace Easing

struct Transform {
    Vector3 translation;
    Vector3 scale;
    float rotation;

    Transform() : translation(0, 0, 0), scale(1, 1, 1), rotation(0) {}

    Transform(const Vector3& translation, const Vector3& scale, float rotation)
        : translation(translation), scale(scale), rotation(rotation) {}

    Transform(const Transform& other)
        : translation(other.translation), scale(other.scale), rotation(other.rotation) {}

    Transform& operator=(const Transform& other) {
        translation = other.translation;
        scale = other.scale;
        rotation = other.rotation;
        return *this;
    }

    Matrix4 toMatrix() const {
        Matrix4 matrix = Matrix4::identity();
        matrix = matrix.translate(translation.x, translation.y, translation.z);
        // matrix = matrix.scale();
        // TODO: ADD ROTATION
        // matrix = matrix.rotate(rotation, {0, 0, 1});
        return matrix;
    }
};

// Pure, stateless animation
// Simple describes a single animation frame
struct AnimationFrame {
    Transform start;
    Transform end;

    Easing::Function easing;

    AnimationFrame() : start(), end(), easing(Easing::linear) {}

    AnimationFrame(const Transform& start, const Transform& end, Easing::Function easing)
        : start(start), end(end), easing(easing) {}

    AnimationFrame(const AnimationFrame& other) : start(other.start), end(other.end), easing(other.easing) {}

    AnimationFrame& operator=(const AnimationFrame& other) {
        start = other.start;
        end = other.end;
        easing = other.easing;
        return *this;
    }
};

// "In Vitro" stateful animation
struct AnimationClip {
    AnimationFrame frame;
    Transform interpolated;

    Seconds duration;
    Seconds elapsed;

    AnimationClip(const AnimationFrame& frame, Seconds duration)
        : frame(frame), duration(duration), elapsed(0) {}

    void update(Seconds delta) {
        elapsed += delta;

        Seconds alpha = elapsed / duration;

        interpolated.translation =
            Easing::apply(frame.easing, frame.start.translation, frame.end.translation, alpha);
        interpolated.scale = Easing::apply(frame.easing, frame.start.scale, frame.end.scale, alpha);
        interpolated.rotation =
            Easing::apply(frame.easing, frame.start.rotation, frame.end.rotation, alpha);
    }

    bool isFinished() const { return elapsed >= duration; }
};

// Contains a sequence of animation clips
class Animation {
    std::vector<AnimationClip> clips;
    unsigned index = 0;

  public:

    Animation() : index(0) {}

    void add(const AnimationFrame& frame, Seconds duration) {
        clips.push_back(AnimationClip(frame, duration));
    }    

    void update(Seconds delta) {
        auto& frame = clips[index];
        frame.update(delta);

        if (frame.isFinished()) {
            index = (index + 1) % clips.size();
            frame.elapsed = Seconds::zero();
        }
    }

    const Transform& getTransform() const {
        return clips[index].interpolated;
    }
};
